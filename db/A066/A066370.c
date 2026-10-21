/*
 * A066370  Quadruply triangular numbers.
 * https://oeis.org/A066370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000217(A000217(A000217(A000217(n))))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 30, u128 n <= 489,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t triangular(seq_t x)
{
    /* Compute x*(x+1)/2 safely by dividing first. */
    return (x & 1) ? (x * ((x + 1) / 2)) : ((x / 2) * (x + 1));
}

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* t0 = n */
    t = triangular(t);           /* t1 = T(n) */
    t = triangular(t);           /* t2 = T(T(n)) */
    t = triangular(t);           /* t3 = T(T(T(n))) */
    t = triangular(t);           /* t4 = T(T(T(T(n)))) */
    return t;
}
