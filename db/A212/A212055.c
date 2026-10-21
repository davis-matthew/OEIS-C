/*
 * A212055  Number of (w,x,y,z) with all terms in {1,...,n} and w*x<=y*z+2.
 * https://oeis.org/A212055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of (w,x,y,z) in {1..n} with w*x <= y*z + 2
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* n is an unsigned integer index (idx_t); seq_t is the integer type for
 * arithmetic (may be 8–128 bits or arbitrary precision). */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0. */
    if (n == 0)
        return (seq_t)0;

    /* n may be large; use size_t for array dimensions. */
    size_t nsize = (size_t)n;
    size_t array_len = nsize * nsize + 1;

    /* Frequency array for the products w*x, size n^2+1.
     * Using uint16_t is sufficient: the maximum frequency of a product
     * ≤ n^2 is bounded by sqrt(n^2) = n ≤ 65535 for all n <= 65535,
     * which covers all practical n in the required ranges. */
    uint16_t *freq = (uint16_t *)calloc(array_len, sizeof(uint16_t));
    if (!freq)
        return (seq_t)0;  /* allocation failure: return 0 as a safe fallback */

    /* Count all products w*x. */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            size_t p = (size_t)(w * x);
            ++freq[p];
        }
    }

    /* Count violating quadruples: w*x > y*z + 2. */
    seq_t cum = (seq_t)0;   /* cumulative sum of freq[1..p-1] */
    seq_t b   = (seq_t)0;   /* number of violating quadruples */

    for (size_t p = 1; p < array_len; ++p) {
        uint16_t f_p = freq[p];
        if (p >= 3) {
            /* pref = sum_{q=1}^{p-3} freq[q] */
            seq_t pref = cum - (seq_t)freq[p - 1] - (seq_t)freq[p - 2];
            b += (seq_t)f_p * pref;
        }
        cum += (seq_t)f_p;
    }

    /* a(n) = n^4 - b */
    seq_t n_seq = (seq_t)n;
    seq_t n4 = n_seq * n_seq * n_seq * n_seq;
    seq_t result = n4 - b;

    free(freq);
    return result;
}
