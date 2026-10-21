/*
 * A088058  Duplicate of A062392.
 * https://oeis.org/A088058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = ( (n(n+1)-1) * (n(n+1)) ) / 2
 * Range:     i8 n <= 3, i16 n <= 14, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (sequence A088058 – indices start at 1) */
seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n * ((seq_t)n + 1);   /* t = n*(n+1) */
    seq_t idx = t - 1;                       /* idx = n*(n+1)-1 */
    return idx * (idx + 1) / 2;              /* triangular number T_idx */
}
