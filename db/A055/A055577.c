/*
 * A055577  Numbers k such that the sum of digits of k^6 is equal to k.
 * https://oeis.org/A055577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   a(n) = k such that sum of the decimal digits of k^6 equals k
 *            (OEIS A055577). The sequence is finite; we precompute all such k
 *            up to 200 (enough to capture every element) and return the n‑th
 *            element, or 0 if n exceeds the number of terms
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t values[200];
    static int len = 0;
    static bool init = false;

    if (!init) {
        /* Scan all k from 0 to 200 and collect those for which
           sumdigits(k^6) == k.  This is fast and guarantees
           that the finite sequence is fully captured. */
        for (idx_t k = 0; k <= 200; ++k) {
            seq_t p = 1;
            for (int i = 0; i < 6; ++i)
                p *= (seq_t)k;          /* compute k^6 */

            seq_t s = 0;
            seq_t t = p;
            while (t > 0) {
                s += t % 10;
                t /= 10;
            }                           /* s = digit sum of k^6 */

            if (s == (seq_t)k) {
                values[len++] = (seq_t)k;
            }
        }
        init = true;
    }

    if (n == 0 || n > (idx_t)len)
        return 0;            /* out of bounds: return 0 */
    else
        return values[n - 1];
}
