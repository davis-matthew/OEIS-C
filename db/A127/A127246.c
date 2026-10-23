/*
 * A127246  Row sums of a Thue-Morse related triangle.
 * https://oeis.org/A127246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + length of the consecutive block of 1's in the
 *            Thue–Morse sequence that ends at index n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t run_len = 0;   /* length of trailing 1's ending at current i */

    for (idx_t i = 0; i <= n; ++i) {
        /* Thue–Morse value at i: parity of popcount(i) (0 => 0, 1 => 1). */
        bool t = (__builtin_parityll((unsigned long long)i) != 0);
        if (t)
            ++run_len;
        else
            run_len = 0;
    }

    /* a(n) = run_len + 1 */
    return (seq_t)(run_len + 1);
}
