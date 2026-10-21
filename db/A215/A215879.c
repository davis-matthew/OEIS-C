/*
 * A215879  Written in base 3, n ends in a(n) consecutive nonzero digits.
 * https://oeis.org/A215879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = number of trailing non‑zero digits in the base‑3
 *            representation of n. Recurrence: a(3n) = 0; a(3n+1) = a(3n+2) =
 *            a(n) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;          /* temporary copy of the index */
    seq_t result = 0;     /* the output value as seq_t */

    /* Count how many least significant base‑3 digits are non‑zero.
       Stop when a zero digit is encountered or when the number becomes 0. */
    while (t > 0 && t % 3 != 0) {
        result++;
        t /= 3;
    }

    return result;
}
