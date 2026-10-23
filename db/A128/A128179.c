/*
 * A128179  A097807 * A002260.
 * https://oeis.org/A128179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1-(-1)^j)*i/2 with i = n - t*(t+1)/2, j = ((t*t + 3*t +
 *            4)/2) - n, t = floor((sqrt(8n-7)-1)/2). In other words: a(n)
 *            equals i when j is odd, otherwise 0
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>        /* for uint64_t – seq_t and idx_t are provided externally */

seq_t a(idx_t n)
{
    /* Find t such that t*(t+1)/2 < n but (t+1)*(t+2)/2 >= n. */
    idx_t t = 0;
    while ((t + 1) * (t + 2) / 2 < n) {
        t++;
    }

    /* Compute i = n - t*(t+1)/2. */
    seq_t i = (seq_t)n - (seq_t)t * (t + 1) / 2;

    /* Compute j = ((t*t + 3*t + 4)/2) - n. */
    idx_t j = ((t * t + 3 * t + 4) / 2) - n;

    /* Return i if j is odd; otherwise return 0. */
    return (j & 1) ? i : 0;
}
