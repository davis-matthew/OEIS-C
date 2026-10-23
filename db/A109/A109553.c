/*
 * A109553  a(n) = floor(4076/(10-n)^2/0.51099906).
 * https://oeis.org/A109553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, less, dumb
 * Formula:   a(n) = floor(4076/(9-n)^2 / 0.51099906)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute (9-n) as signed to handle n > 9 */
    long long diff = (long long)9 - (long long)n;
    /* square, results fit in 64 bits for n up to at least 10^4 */
    unsigned long long d = (unsigned long long)(diff * diff);

    /* undefined at n = 9 (denominator zero) */
    if (d == 0)
        return (seq_t)0;          /* behaviour for undefined index */

    /* compute using integer arithmetic: a(n) = floor(4076 * 1e8 / (d * 51099906)) */
    const unsigned long long num = 4076ULL * 100000000ULL;   /* 407600000000 */
    const unsigned long long den = d * 51099906ULL;

    return (seq_t)(num / den);
}
