/*
 * A178972  Number of ways to place 2 nonattacking amazons (superqueens) on an
 *          n X n toroidal board.
 * https://oeis.org/A178972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(2n^2 - 8n - 9 + (-1)^n)/4 for n>=5, 0 otherwise
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 305, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    seq_t nn = (seq_t)n * (seq_t)n;                          /* n^2 */
    seq_t base = 2 * nn - 8 * (seq_t)n - 9;                  /* 2n^2 - 8n - 9 */

    if (n & 1) base -= 1;   /* add (-1)^n == -1 when n is odd */
    else       base += 1;   /* add 1 when n is even */

#if defined(__SIZEOF_INT128__)
    unsigned __int128 tmp = (unsigned __int128)nn * (unsigned __int128)base;
    return (seq_t)(tmp / 4);
#else
    return (nn * base) / 4;
#endif
}
