/*
 * A037596  Numbers written in base 4 whose digits are, in order, the first n
 *          terms of the periodic sequence with initial period 1,0,3.
 * https://oeis.org/A037596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((103/999) * 10^n) * Recurrence: a(1) = 1; for n ≥
 *            2, * a(n) = 10 * a(n-1) + d[(n-1) mod 3], * where the digit
 *            pattern d is {1, 0, 3} repeating
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is outside the defined offset; return 0 for safety. */
    if (n == 0u) return 0;

    static const unsigned digits[3] = { 1, 0, 3 };
    seq_t value = 0;          /* start with zero decimal value */

    /* Accumulate digits: each step appends a new digit to the right. */
    for (idx_t i = 0; i < n; ++i) {
        value = value * 10 + digits[i % 3];
    }
    return value;
}
