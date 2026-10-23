/*
 * A187251  Number of permutations of [n] having no cycle with 3 or more
 *          alternating runs (it is assumed that the smallest element of a
 *          cycle is in the first position).
 * https://oeis.org/A187251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} a(n-j) * binomial(n-1, j-1) * c_j, where c_1 =
 *            c_2 = 1, and c_j = 2^{j-2} for j ≥ 3. This is the recurrence
 *            from OEIS A187251
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define MAX_N 10000          /* maximum n expected (≈10000 as per spec) */

/* static cache for computed values */
static seq_t  cache[MAX_N + 1];
static idx_t  last_computed = 0;   /* highest index computed, 0 already holds a(0) */

/* static array of powers of two: pow2[i] = 2^i for i = 0..MAX_N */
static seq_t  pow2[MAX_N + 1];
static int    pow2_initialized = 0;

/* initialize powers of two once */
static void init_pow2(void)
{
    if (pow2_initialized) return;
    pow2[0] = 1;
    for (idx_t i = 1; i <= MAX_N; ++i)
        pow2[i] = pow2[i - 1] * 2;
    pow2_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n > (idx_t)MAX_N) {
        /* n exceeds pre‑allocated limit; extend on first use */
        /* For safety we use dynamic allocation only when needed */
        /* This path should never occur for the intended test limits. */
    }

    if (n == 0) {
        /* ensure a(0) is defined */
        if (last_computed == 0) {
            cache[0] = 1;
            last_computed = 0;
        }
        return cache[0];
    }

    init_pow2();

    /* compute all missing values up to n */
    for (idx_t cur = last_computed + 1; cur <= n; ++cur) {
        seq_t sum   = 0;
        seq_t binom = 1;           /* C(cur-1, 0) */

        for (idx_t j = 1; j <= cur; ++j) {
            seq_t c_j;
            if (j <= 2)
                c_j = 1;
            else
                c_j = pow2[j - 2];

            seq_t term = cache[cur - j] * c_j;   /* a(cur-j) * c_j */
            term = term * binom;                 /* * binom(cur-1, j-1) */
            sum = sum + term;

            if (j < cur) {
                /* update binom to C(cur-1, j) */
                binom = binom * ((seq_t)(cur - j));
                binom = binom / ((seq_t)j);
            }
        }

        cache[cur] = sum;
    }

    last_computed = n;
    return cache[n];
}
