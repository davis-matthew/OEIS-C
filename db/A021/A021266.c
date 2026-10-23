/*
 * A021266  Decimal expansion of 1/262.
 * https://oeis.org/A021266
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

/* Result: nth decimal digit of 1/262 (0‑based). */
/* Example: a(0) = 0, a(1) = 0, a(2) = 3, ...  */

seq_t a(idx_t n)
{
    unsigned long long rem = 1;
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ull;
        digit = (seq_t)(rem / 262ull);
        rem %= 262ull;
    }
    return digit;
}
