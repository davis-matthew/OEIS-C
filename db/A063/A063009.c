/*
 * A063009  Write n in binary then square as if written in base 10.
 * https://oeis.org/A063009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = (decimal value of the binary representation of n)²
 *            Recurrence: a(2n) = 100 · a(n) a(2n+1) = 100 · a(n) + 20 ·
 *            decimal(binary(n)) + 1 (The explicit computation below uses the
 *            direct definition.)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 31, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute a(n) for OEIS A063009 */
seq_t a(idx_t n)
{
    if (n == 0)            /* n=0 gives 0² = 0 */
        return 0;

    /* find the index of the most significant set bit of n */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        msb++;
        tmp >>= 1;
    }
    msb--;                   /* msb is 0‑based */

    /* build the decimal value of the binary string of n */
    seq_t dec = 0;
    for (idx_t i = msb; ; --i) {
        dec = dec * 10 + ((n >> i) & 1);
        if (i == 0) break;  /* avoid unsigned underflow */
    }

    /* square the decimal value */
    return dec * dec;
}
