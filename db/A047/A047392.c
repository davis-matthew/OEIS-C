/*
 * A047392  Numbers that are congruent to {0, 1, 3, 5} mod 7.
 * https://oeis.org/A047392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(3*n/4-3/4) - 1
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived: a(n) = n-1 + (3*n-3)/4 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t early to avoid mixing types. */
    seq_t tn = (seq_t)n;

    /* Compute floor((3*n-3)/4) using only integer arithmetic. */
    seq_t part = 3 * (tn - 1) / 4;

    /* Final formula: n-1 + part. */
    return tn - 1 + part;
}
