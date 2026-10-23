/*
 * A154225  List of pairs: {n*(n + 1)*(2*n + 1)/6, (n!)^2}.
 * https://oeis.org/A154225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = ((n%2==0) ? (n/2)*(n/2+1)*(n+1)/6 : ((n-1)/2)!^2)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* static storage for factorial values */
static seq_t *fact = NULL;          /* factorial array: fact[i] = i! */
static size_t fact_len = 0;         /* number of computed factorials (max index + 1) */
static size_t fact_cap = 0;         /* allocated capacity of 'fact' */

/*ensure that 'fact' can hold values up to index 'up_to' (inclusive) */
static void ensure_factorial_capacity(size_t up_to)
{
    if (fact_cap <= up_to) {
        size_t new_cap = fact_cap ? fact_cap * 2 : 1;
        while (new_cap <= up_to) new_cap <<= 1;
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
        /* initialize factorial array if it was just created */
        if (fact_len == 0) {
            fact[0] = (seq_t)1;
            fact_len = 1;
        }
    }
}

/* compute a(n) */
seq_t a(idx_t n)
{
    /* even index -> sum of squares up to n/2 */
    if ((n & 1ULL) == 0) {
        idx_t k = n >> 1;                 /* k = n/2 */
        seq_t k_t = (seq_t)k;
        seq_t val = k_t * (k_t + 1);       /* k*(k+1) */
        val = val * (2 * k_t + 1);         /* * (2k+1) */
        val = val / 6;                    /* /6  */
        return val;
    }

    /* odd index -> square of factorial of (n-1)/2 */
    idx_t k = (n - 1) >> 1;                /* k = (n-1)/2 */

    if ((size_t)k >= fact_len) {
        if ((size_t)k >= fact_cap) {
            ensure_factorial_capacity((size_t)k);
        }
        /* compute factorials up to k */
        for (size_t i = fact_len; i <= (size_t)k; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = (size_t)k + 1;
    }

    seq_t sq = fact[(size_t)k] * fact[(size_t)k];
    return sq;
}
