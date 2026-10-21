/*
 * A222942  Number of n X 4 0..4 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..4 introduced
 *          in row major order.
 * https://oeis.org/A222942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) + 136*a(n-2) - 512*a(n-3) (for n>5) Initial
 *            values: a(1)=7, a(2)=273, a(3)=5880, a(4)=130680, a(5)=2883000
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms (1‑based index). */
    /* Size is sufficient for all tests (n up to ~10000). */
    static seq_t cache[100001] = {0};
    static idx_t cached_len = 0;       /* greatest index computed so far */

    if (n < 1) {
        return (seq_t)0;               /* undefined below offset */
    }

    /* Compute missing terms up to n if necessary. */
    if (n > cached_len) {
        idx_t i;
        for (i = cached_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[1] = (seq_t)7;
            } else if (i == 2) {
                cache[2] = (seq_t)273;
            } else if (i == 3) {
                cache[3] = (seq_t)5880;
            } else if (i == 4) {
                cache[4] = (seq_t)130680;
            } else if (i == 5) {
                cache[5] = (seq_t)2883000;
            } else {
                cache[i] = (seq_t)17 * cache[i-1]
                         + (seq_t)136 * cache[i-2]
                         - (seq_t)512 * cache[i-3];
            }
        }
        cached_len = i - 1;
    }

    return cache[n];
}
