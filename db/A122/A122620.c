/*
 * A122620  a(n) = n_{n+1}.
 * https://oeis.org/A122620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n_{n+1}
 * Range:     u8 n <= 50, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is obtained by interpreting the decimal
 * representation of n as a number written in base (n+1).
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n + 1;  /* base is n+1 */
    seq_t result = 0;

    /* Extract decimal digits of n, least‑significant first. */
    unsigned digits[20];   /* 64‑bit unsigned max 20 decimal digits */
    size_t dlen = 0;
    idx_t temp = n;
    do
    {
        digits[dlen++] = (unsigned)(temp % 10);
        temp /= 10;
    } while (temp != 0);

    /* Build the value in base (n+1) from most → least significant digit. */
    for (size_t i = dlen; i-- > 0; )
    {
        result = result * base + (seq_t)digits[i];
    }

    return result;
}
