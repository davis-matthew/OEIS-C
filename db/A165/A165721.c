/*
 * A165721  Integers of the form k*(k+13)/12.
 * https://oeis.org/A165721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k(k+13)/12 where k runs through the integers 12*m + r
 *            with r in {3,8,11,12} (i.e. r = {3,8,11,0} mod 12) for m =
 *            0,1,2,.... The offset of the sequence is 1, so a(1) corresponds
 *            to k = 3
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for undefined n = 0 (not part of the sequence). */
    if (n == 0)
        return (seq_t)0;

    /* j = n-1 gives the zero‑based position in the sequence. */
    idx_t j = n - 1;

    /* base = 12 * floor(j/4) gives the 12‑multiple part of k. */
    idx_t base = 12 * (j / 4);

    /* r cycles through 3, 8, 11, 12. */
    idx_t r;
    switch (j % 4) {
    case 0: r = 3;  break;
    case 1: r = 8;  break;
    case 2: r = 11; break;
    default: r = 12; /* equivalent to residue 0 modulo 12 */
    }

    /* Compute k. */
    idx_t k = base + r;

    /* Convert to seq_t for arithmetic. */
    seq_t ks = (seq_t)k;

    /* Compute the term: k(k+13)/12, the division is exact. */
    seq_t term = ks * (ks + (seq_t)13) / (seq_t)12;

    return term;
}
