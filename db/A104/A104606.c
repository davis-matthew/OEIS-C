/*
 * A104606  Write the natural numbers in base 2 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = bit at position n*(n+1)/2 in the binary concatenation of
 *            natural numbers
 * Range:     u8 n <= 22, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safe guard, unused for OEIS offset 1 */
    seq_t m = (seq_t)n * ((seq_t)n + 1) / 2;  /* position of the desired bit */

    /* Find the minimal bit-length l such that the cumulative bits up to numbers of
       length l-1 is < m <= cumulative bits up to length l. */
    idx_t  l   = 1;                            /* current bit-length (1‑based)     */
    seq_t  pow2 = (seq_t)1;                    /* 2^(l-1)                         */
    seq_t  cum  = 0;                           /* bits counted so far              */

    while (cum + ((seq_t)l * pow2) < m) {
        cum += ((seq_t)l * pow2);
        pow2 <<= 1;     /* now pow2 == 2^l  for the next iteration   */
        l++;
    }

    /* m lies in the block of l‑bit numbers starting at 2^(l-1).  */
    seq_t offset      = m - cum - 1;          /* zero‑based offset within the block  */
    seq_t num_offset  = offset / (seq_t)l;    /* which number in the block           */
    seq_t bit_offset  = offset % (seq_t)l;    /* which bit of that number (MSB first) */
    seq_t number      = pow2 + num_offset;    /* the actual number whose bit we need */

    idx_t shift = (idx_t)(l - 1 - (idx_t)bit_offset);
    seq_t bit = (number >> shift) & 1;

    return bit;
}
