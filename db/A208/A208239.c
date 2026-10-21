/*
 * A208239  Triangle read by rows: T(n,m) = n + k - n/k, where k is the m-th
 *          divisor of n; 1 <= m <= tau(n).
 * https://oeis.org/A208239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth term of the triangle read by rows where T(n,m) =
 *            n + k - n/k and k is the m‑th divisor of n
 * Range:     u8 n <= 648, u16 n <= 1098, u32 n <= 1098, u64 n <= 1098,
 *            u128 n <= 1098, big any n
 * Helpers:   cmp_idx, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/cmp_idx.h"

/* ----------  helper functions  ---------- */

/* dynamic array holding cumulative divisor counts.
   cum[i] = Σ_{t=1..i} τ(t), with cum[0] = 0.   */
static idx_t *cum = NULL;   /* pointer to the array */
static size_t cum_cap = 0;   /* allocated capacity */
static size_t cum_len = 0;   /* number of valid elements (>=1) */

/* ensure cum contains entries up to at least index 'target' */
static void ensure_cum(idx_t target)
{
    if (cum == NULL) {
        cum_cap = 16;
        cum = (idx_t*)calloc(cum_cap, sizeof(*cum));
        if (!cum) abort();
        cum[0] = 0;
        cum_len = 1;          /* we have cum[0] */
    }
    while (cum_len <= (size_t)target) {
        if (cum_len >= cum_cap) {          /* grow the array */
            size_t new_cap = cum_cap * 2;
            if (new_cap < cum_len + 1) new_cap = cum_len + 1;
            idx_t *tmp = (idx_t*)realloc(cum, new_cap * sizeof(*cum));
            if (!tmp) abort();
            cum = tmp;
            cum_cap = new_cap;
        }
        idx_t i = cum_len;                  /* next integer to process */
        uint64_t p[64], e[64];
        size_t pc;
        factor_u64((uint64_t)i, p, e, &pc);  /* prime factorization */

        idx_t tau = 1;
        for (size_t j = 0; j < pc; ++j)
            tau *= (idx_t)(e[j] + 1);

        cum[i] = cum[i-1] + tau;            /* cumulative sum */
        cum_len++;
    }
}

/* comparison function for qsort on idx_t array */

/* generate divisors of a number given its prime factorization.
   The array 'arr' must have space for at least tau(n) elements. */
static void gen_divs_rec(size_t idx, idx_t cur,
                         const idx_t *pr, const idx_t *ex, size_t cnt,
                         idx_t *arr, size_t *pos)
{
    if (idx == cnt) {
        arr[(*pos)++] = cur;
    } else {
        idx_t p = pr[idx];
        idx_t mult = 1;
        for (idx_t e = 0; e <= ex[idx]; ++e) {
            gen_divs_rec(idx + 1, cur * mult, pr, ex, cnt, arr, pos);
            mult *= p;
        }
    }
}

/* ----------  main sequence function  ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below offset */

    /* make sure we can look up the row containing position n */
    ensure_cum(n);

    /* binary search for smallest r such that cum[r] >= n */
    idx_t low = 1, high = (idx_t)(cum_len - 1);
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cum[mid] >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t m = n - cum[r-1];              /* position within row r (1‑based) */

    /* number of divisors of r */
    idx_t tau_r = cum[r] - cum[r-1];
    size_t tau_sz = (size_t)tau_r;

    /* factor r to obtain all its divisors */
    uint64_t primes64[64], exps64[64];
    size_t pc;
    factor_u64((uint64_t)r, primes64, exps64, &pc);

    idx_t pr[64], ex[64];
    for (size_t i = 0; i < pc; ++i) {
        pr[i] = (idx_t)primes64[i];
        ex[i] = (idx_t)exps64[i];
    }

    idx_t *divs = (idx_t*)malloc(tau_sz * sizeof(*divs));
    if (!divs) abort();
    size_t pos = 0;
    gen_divs_rec(0, (idx_t)1, pr, ex, pc, divs, &pos);
    qsort(divs, tau_sz, sizeof(*divs), cmp_idx);

    idx_t k = divs[m-1];
    seq_t result = (seq_t)(r + k - r / k);

    free(divs);
    return result;
}
