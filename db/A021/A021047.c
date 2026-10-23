/*
 * A021047  Decimal expansion of 1/43.
 * https://oeis.org/A021047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit in the decimal expansion of 1/43 after the
 *            decimal point. The sequence is produced by long division: start
 *            with remainder 1, then repeat: remainder *= 10; digit =
 *            remainder / 43; remainder %= 43
 * Range:     u8 n <= 5, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns a sequence element with external linkage as required. */
seq_t a(idx_t n)
{
    /* The maximum value of the remainder is 42, so multiplying by 10 keeps
       the intermediate value well within the range of seq_t (which is
       at least 64‑bit). */
    seq_t remainder = 1;
    seq_t digit;

    /* Run the division process n+1 times to reach the nth digit. */
    for (idx_t i = 0; i <= n; ++i) {
        remainder *= (seq_t)10;
        digit = remainder / (seq_t)43;
        remainder %= (seq_t)43;
    }
    return digit;
}
