/*
 * A060757  a(n) = 4^(n^2).
 * https://oeis.org/A060757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4^(n^2)
 * Computes a(n) for arbitrary‑precision or fixed‑width seq_t.
 */
seq_t a(idx_t n)
{
    /* exponent n^2 as seq_t */
    seq_t exp = (seq_t)n * (seq_t)n;

    /* 4 as seq_t */
    seq_t base = (seq_t)4;

    /* 1 as seq_t */
    seq_t result = (seq_t)1;

    /* binary exponentiation */
    while (exp > 0) {
        if (exp & (seq_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return result;
}
