/*
 * A018359  Divisors of 258.
 * https://oeis.org/A018359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth divisor of 258 in ascending order */
seq_t a(idx_t n)
{
    static const seq_t divisors[8] = { 1, 2, 3, 6, 43, 86, 129, 258 };
    if (n < 1 || n > 8) return (seq_t)0;
    return divisors[n - 1];
}
