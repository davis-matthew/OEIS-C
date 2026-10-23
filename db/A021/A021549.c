/*
 * A021549  Decimal expansion of 1/545.
 * https://oeis.org/A021549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit of the decimal expansion of 1/545, computed by the
 *            standard long‑division remainder method
 * Range:     u8 n <= 109, u16 n <= 109, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned long long den = 545ULL;   /* constant denominator */
    unsigned long long r = 1;                /* current remainder (1 initially) */
    seq_t digit = 0;                         /* value to return */

    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10ULL;                     /* bring down a zero */
        digit = (seq_t)(r / den);           /* next digit */
        r = r % den;                        /* new remainder */
    }
    return digit;
}
