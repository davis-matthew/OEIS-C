/*
 * A038322  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*8^j.
 * https://oeis.org/A038322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 11^(i-j) * 8^j where i =
 *            floor((sqrt(8n+1)-1)/2) and j = n - i*(i+1)/2 (triangle entries
 *            read row‑wise, offset 0)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 38, u64 n <= 139, u128 n <= 505,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial, works for any seq_t */
static seq_t binomial_u64(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t t = 1; t <= k; ++t) {
        r = r * (seq_t)(n - k + t);
        r = r / (seq_t)t;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find row index i and column j by successive subtraction.
       This avoids any square‑root on large numbers and is O(sqrt(n)). */
    idx_t rem = n;
    idx_t i = 0;
    while (rem >= (i + 1)) {
        rem -= (i + 1);
        ++i;
    }
    idx_t j = rem;                    /* 0 ≤ j ≤ i */

    /* Binomial coefficient */
    seq_t bin = binomial_u64(i, j);

    /* Powers of 11 and 8 */
    seq_t pow11 = 1;
    for (idx_t t = 0; t < i - j; ++t) {
        pow11 = pow11 * (seq_t)11;
    }

    seq_t pow8 = 1;
    for (idx_t t = 0; t < j; ++t) {
        pow8 = pow8 * (seq_t)8;
    }

    /* Final result */
    return bin * pow11 * pow8;
}
