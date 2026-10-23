/*
 * A011666  A binary m-sequence: expansion of reciprocal of x^6+x^5+x^4+x+1.
 * https://oeis.org/A011666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0..4)=0, a(5)=1; for n>=6, a(n)=a(n-1)+a(n-4)+a(n-5)+a(n-6)
 *            (mod 2)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)0;
    if (n == 5) return (seq_t)1;

    /* Circular buffer for the last six values, index modulo 6 */
    seq_t buf[6] = {0, 0, 0, 0, 0, 1};

    for (idx_t i = 6; i <= n; ++i) {
        /* XOR is addition modulo 2 */
        seq_t new_val = buf[(i - 1) % 6] ^ buf[(i - 4) % 6] ^
                        buf[(i - 5) % 6] ^ buf[(i - 6) % 6];
        buf[i % 6] = new_val;
    }

    return buf[n % 6];
}
