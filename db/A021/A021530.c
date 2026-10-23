/*
 * A021530  Decimal expansion of 1/526.
 * https://oeis.org/A021530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth digit in the decimal expansion of 1/526 (a(0) = first digit after the decimal point).
   Computed by long division: remainder starts at 1, then at each step r *= 10; digit = r / 526; r %= 526. */
seq_t a(idx_t n)
{
    uint64_t r = 1;          /* current remainder of 1 / 526 */
    uint8_t digit = 0;       /* will hold a digit 0‑9 */

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;
        digit = r / 526;
        r %= 526;
    }
    return (seq_t)digit;
}
