/*
 * A182873  First differences of the Ramanujan primes, A104272.
 * https://oeis.org/A182873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = R_{n+1} - R_n, where R_k is the k‑th Ramanujan prime
 * Range:     u8 n <= 1043, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------------------------------------------------------- */
/* static state for memoisation                                 */
/* ---------------------------------------------------------- */
static seq_t *diff_arr = NULL;        /* diff_arr[1..] = a(n)         */
static size_t  diff_cap = 0;          /* capacity of diff_arr         */
static idx_t   max_computed_n = 0;    /* largest n for which diff is computed */

/* ---------------------------------------------------------- */
/*  Estimate an upper bound for R_{k}  (k = n+1)                */
/* ---------------------------------------------------------- */
static idx_t estimate_limit(idx_t k)
{
    /* For k <= 10000 a bound of 30*k + 1000 is safe */
    return k * 30 + 1000;
}

/* ---------------------------------------------------------- */
/*  Compute a(n) for all n up to target (inclusive)           */
/*  (recomputed from scratch when a larger target is needed)  */
/* ---------------------------------------------------------- */
static void compute_to(idx_t target)
{
    if (target <= max_computed_n)
        return;                         /* already done */

    idx_t target_k = target + 1;           /* we need R_{1} … R_{k} , k = target+1 */

    /* ---- 1. Sieve primes up to an upper bound ---------------- */
    idx_t limit = estimate_limit(target_k);
    size_t lim = (size_t)limit;            /* for array indices */

    bool *is_comp = (bool*)calloc(lim + 1, 1);   /* 0 = prime candidate */
    if (!is_comp) { abort(); }

    for (size_t i = 2; i * i <= lim; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= lim; j += i) {
                is_comp[j] = true;
            }
        }
    }

    /* Count primes and collect them */
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!is_comp[i]) ++prime_cnt;

    uint64_t *primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) { free(is_comp); abort(); }

    size_t idxp = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!is_comp[i]) primes[idxp++] = (uint64_t)i;

    /* ---- 2. Prefix π(x) array -------------------------------- */
    size_t *pi = (size_t*)malloc((lim + 1) * sizeof(size_t));
    if (!pi) { free(is_comp); free(primes); abort(); }

    size_t pi_val = 0;
    for (size_t i = 0; i <= lim; ++i) {
        if (i >= 2 && !is_comp[i]) ++pi_val;
        pi[i] = pi_val;
    }

    /* ---- 3. Compute f(x) = π(x) – π(⌊x/2⌋)  for  x = 2 .. lim ----- */
    size_t f_len = lim - 1;              /* for x=2..lim */
    seq_t *f = (seq_t*)malloc(f_len * sizeof(seq_t));
    if (!f) { free(is_comp); free(primes); free(pi); abort(); }

    for (size_t x = 2; x <= lim; ++x) {
        seq_t val = (seq_t)(pi[x] - pi[x / 2]);
        f[x - 2] = val;
    }

    /* ---- 4. Suffix minima of f -------------------------------- */
    for (size_t i = f_len - 2; (long long)i >= 0; --i) {
        seq_t v = f[i];
        if (f[i + 1] < v) v = f[i + 1];
        f[i] = v;
    }

    /* ---- 5. Find Ramanujan primes R_1 .. R_k ------------------- */
    size_t *R = (size_t*)malloc((target_k + 1) * sizeof(size_t));
    if (!R) { free(is_comp); free(primes); free(pi); free(f); abort(); }

    seq_t *new_diff = (seq_t*)malloc((target_k + 1) * sizeof(seq_t)); /* 1‑based */
    if (!new_diff) { free(is_comp); free(primes); free(pi); free(f); free(R); abort(); }

    size_t needed = 1;
    for (size_t x = 2; x <= lim && needed <= target_k; ++x) {
        if (f[x - 2] >= needed) {
            R[needed] = (size_t)x;
            if (needed > 1)
                new_diff[needed - 1] = (seq_t)x - R[needed - 1];
            ++needed;
        }
    }

    /* ---- 6. Cleanup temporary allocations --------------------- */
    free(is_comp);
    free(primes);
    free(pi);
    free(f);
    free(R);

    /* ---- 7. Replace the global cache -------------------------- */
    free(diff_arr);                      /* old cache */
    diff_arr = new_diff;
    diff_cap = target_k + 1;             /* indices 0 .. target_k */
    max_computed_n = target;
}

/* ---------------------------------------------------------- */
/*  External function required by the test harness            */
/* ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* undefined but harmless */
    compute_to(n);
    return diff_arr[n];
}
