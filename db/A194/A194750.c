/*
 * A194750  Number of k such that {k*e} < {n*e}, where { } = fractional part.
 * https://oeis.org/A194750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with 0 ≤ k < n such that {k·e} < {n·e}
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 6, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Scaling constants for a rational approximation of e (no floating‑point). */
#define SCALE_E 1000000000000000000ULL          /* 1e18  */
#define E_APPROX 2718281828459045235ULL         /* floor(e·SCALE_E) */

/* --------- global cache --------------------------------------------------- */
static seq_t *a_cache = NULL;          /* a values for indices < cache_limit   */
static size_t cache_limit = 0;         /* number of indices stored (max+1)      */

/* --------- Fenwick tree helpers -------------------------------------------- */
static void bit_update(seq_t *bit, size_t idx, size_t n, seq_t val)
{
    for (size_t i = idx + 1; i <= n; i += i & -i)
        bit[i] += val;
}

static seq_t bit_query(seq_t *bit, size_t idx, size_t n)
{
    seq_t sum = 0;
    for (size_t i = idx + 1; i > 0; i -= i & -i)
        sum += bit[i];
    return sum;
}

/* --------- compare function for sorting indices by remainder ---------------- */
static seq_t *rem_global = NULL;      /* set temporarily by init_cache */

static int cmp_idx(const void *a, const void *b)
{
    size_t i = *(const size_t *)a;
    size_t j = *(const size_t *)b;
    seq_t ri = rem_global[i];
    seq_t rj = rem_global[j];
    if (ri < rj) return -1;
    if (ri > rj) return 1;
    /* tie‑break by numerically smaller index to keep deterministic order */
    return (i < j) ? -1 : (i > j) ? 1 : 0;
}

/* --------- build cache up to 'limit' (indices 0 … limit-1) ---------------- */
static void init_cache(size_t limit)
{
    if (limit <= cache_limit)
        return;                       /* already large enough */

    /* free previous cache */
    free(a_cache);

    /* allocate new arrays for computation */
    a_cache = (seq_t *)malloc(limit * sizeof(seq_t));
    if (!a_cache)
        return;                       /* allocation failed */

    seq_t *rem = (seq_t *)malloc(limit * sizeof(seq_t));
    if (!rem) {
        free(a_cache);
        a_cache = NULL;
        return;
    }

    size_t *idx_arr = (size_t *)malloc(limit * sizeof(size_t));
    if (!idx_arr) {
        free(rem);
        free(a_cache);
        a_cache = NULL;
        return;
    }

    /* compute scaled fractional parts: r_k = (k·E_APPROX) mod SCALE_E */
    for (size_t k = 0; k < limit; ++k) {
        unsigned __int128 prod = (unsigned __int128)k * E_APPROX;
        rem[k] = (seq_t)(prod % SCALE_E);
        idx_arr[k] = k;
    }

    /* make rem accessible to cmp_idx */
    rem_global = rem;
    /* sort indices by their remainder (ascending) */
    qsort(idx_arr, limit, sizeof(size_t), cmp_idx);
    rem_global = NULL;        /* no longer needed after sort */

    /* Fenwick tree over indices for counting processed elements */
    seq_t *bit = (seq_t *)calloc(limit + 1, sizeof(seq_t));
    if (!bit) {
        free(idx_arr);
        free(rem);
        free(a_cache);
        a_cache = NULL;
        return;
    }

    /* process indices in order of increasing fractional part */
    for (size_t pos = 0; pos < limit; ++pos) {
        size_t idx = idx_arr[pos];
        seq_t cnt = 0;
        if (idx > 0)
            cnt = bit_query(bit, idx - 1, limit);
        a_cache[idx] = cnt;
        bit_update(bit, idx, limit, 1);
    }

    /* free temporary arrays */
    free(bit);
    free(idx_arr);
    free(rem);

    cache_limit = limit;
}

/* --------- required sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n=0, return 0 safely */

    /* ensure cache contains index n */
    size_t target = (size_t)n + 1;
    if (target > cache_limit) {
        size_t new_limit = (cache_limit > 0) ? cache_limit : 1;
        while (new_limit < target)
            new_limit <<= 1;            /* exponential growth so we reuse work */
        init_cache(new_limit);
    }

    /* if allocation failed and we still can't answer, fall back to brute force */
    if ((size_t)n >= cache_limit) {
        seq_t result = 0;
        unsigned __int128 prod_n = (unsigned __int128)n * E_APPROX;
        seq_t rn = (seq_t)(prod_n % SCALE_E);
        for (size_t k = 0; k < (size_t)n; ++k) {
            unsigned __int128 prod_k = (unsigned __int128)k * E_APPROX;
            seq_t rk = (seq_t)(prod_k % SCALE_E);
            if (rk < rn)
                result += 1;
        }
        return result;
    }

    return a_cache[(size_t)n];
}
