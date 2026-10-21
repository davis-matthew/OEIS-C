/*
 * A075997  a(n) = [n/2] - [n/3] + [n/4] - [n/5] + [n/6] - ..., where [n/k] =
 *          floor(n/k).
 * https://oeis.org/A075997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=2}^{∞} (-1)^k ⌊n/k⌋ (for n ≥ 0, n=0 gives 0)
 * Range:     u8 n <= 815, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The series stops effectively at k = n, because ⌊n/k⌋ = 0 for k > n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ret = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t term = (seq_t)(n / k);
        if ((k & 1) == 0)   /* even k → sign +1 */
            ret += term;
        else                /* odd k  → sign -1 */
            ret -= term;
    }
    return ret;
}
