/*
 * A222220  In the number n, replace all (decimal) digits '1' with '3' and
 *          vice versa.
 * https://oeis.org/A222220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Replace all decimal digits '1' with '3' and vice versa. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;
    seq_t x      = (seq_t)n;          /* copy of the input */

    if (x == 0)
        return 0;

    while (x > 0) {
        seq_t digit = x % 10;
        seq_t new_digit;
        if (digit == 1)
            new_digit = 3;
        else if (digit == 3)
            new_digit = 1;
        else
            new_digit = digit;

        result = result + new_digit * power;
        power  = power * 10;
        x      = x / 10;
    }

    return result;
}
