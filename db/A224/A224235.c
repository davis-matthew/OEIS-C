/*
 * A224235  Decimal expansion of number of yards in a meter.
 * https://oeis.org/A224235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = a(n-42)
 * Range:     u8 n <= 2, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the decimal expansion (including the integer part) of 1/0.9144.
   The rational value is 1250/1143, so the digits repeat with period 42. */
seq_t a(idx_t n)
{
    static seq_t digits[42];
    static int init = 0;
    if (!init) {
        init = 1;
        /* 1/0.9144 = 1250/1143 */
        seq_t num = 1250;
        seq_t den = 1143;
        digits[0] = num / den;          /* integer part */
        seq_t rem  = num % den;         /* remainder for fractional part */
        for (idx_t i = 1; i < 42; ++i) {
            rem = rem * 10;
            digits[i] = rem / den;
            rem = rem % den;
        }
    }
    /* Map n to the 0‑based index of the repeating period. */
    idx_t r = (n - 1) % 42;
    return digits[r];
}
