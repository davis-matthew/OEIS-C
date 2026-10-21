/*
 * A085787  Generalized heptagonal numbers: m*(5*m - 3)/2, m = 0, +-1, +-2
 *          +-3, ...
 * https://oeis.org/A085787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n) + T(floor(n/2)) where T(k) = k*(k+1)/2 is the k-th
 *            triangular number. This yields the generalized heptagonal
 *            numbers in the OEIS sequence A085787
 * Range:     u8 n <= 19, u16 n <= 323, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute T(n) */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* compute floor(n/2) */
    idx_t h = n >> 1;  /* integer division by 2 is the same as floor(n/2) */

    /* compute T(floor(n/2)) */
    seq_t th = (seq_t)h * ((seq_t)h + 1) / 2;

    return tn + th;
}
