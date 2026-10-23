/*
 * A021348  Decimal expansion of 1/344.
 * https://oeis.org/A021348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th decimal digit of 1/344. 1/344 =
 *            0.0029069767441860465116279069767441860465116279069767…
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* remainder after each decimal step, fits in 64 bits */
    uint64_t rem = 1ULL;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i)
    {
        /* perform one long‑division step  */
        uint64_t x = rem * 10ULL;          /* multiply by 10 */
        digit = (seq_t)(x / 344ULL);       /* next digit */
        rem   = x % 344ULL;                /* new remainder */
    }
    return digit;
}
