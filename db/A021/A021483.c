/*
 * A021483  Decimal expansion of 1/479.
 * https://oeis.org/A021483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/479 Computation:
 *            iterate long division: start with remainder 1, for each step
 *            multiply by 10, the digit is remainder / 479, the new remainder
 *            is remainder % 479. The nth digit is obtained after the nth step
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t i;
    unsigned long long r = 1ULL;          /* current remainder < 479   */
    for (i = 0; i <= n; ++i) {
        r *= 10ULL;                      /* scale for next digit      */
        seq_t digit = (seq_t)(r / 479ULL);
        if (i == n)
            return digit;
        r %= 479ULL;                     /* new remainder             */
    }
    return 0; /* unreachable, but keeps compilers happy */
}
