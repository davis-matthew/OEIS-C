/*
 * A080672  Numbers having divisors 2 or 3 or 5 or 7.
 * https://oeis.org/A080672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-162) + 210 for n > 162 (period of 162 with block size
 *            210). Initial values are the numbers ≤210 that have a divisor 2,
 *            3, 5, or 7
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                               /* undefined for n == 0 */

    /* Pre‑compute the first 162 terms (within the first block 1…210). */
    static seq_t base[162];
    static size_t base_len = 0;

    if (base_len == 0) {
        size_t cnt = 0;
        for (idx_t i = 2; i <= 210; ++i) {
            if (i % 2 == 0 || i % 3 == 0 || i % 5 == 0 || i % 7 == 0) {
                base[cnt++] = (seq_t)i;
                if (cnt == 162)
                    break;
            }
        }
        base_len = cnt;         /* should be 162 */
    }

    idx_t offset = (n - 1) / 162;    /* number of complete blocks before n */
    idx_t rem    = (n - 1) % 162;    /* position inside the current block   */

    /* Increment by offset * 210 to reach the correct block. */
    return base[rem] + (seq_t)offset * (seq_t)210;
}
