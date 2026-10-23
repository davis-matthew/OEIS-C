/*
 * A026922  Number of partitions of n into an odd number of parts, the
 *          greatest being 2; also, a(n+3) = number of partitions of n+1 into
 *          an even number of parts, each <=2.
 * https://oeis.org/A026922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/4) – [n == 3 mod 4]
 * Range:     u8 n <= 286, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This matches A026922: number of partitions of n into an odd number of parts,
   the greatest being 2. */

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t res = ((seq_t)n + 2) / 4;   /* floor((n+2)/4) */
    if ((n & 3) == 3)                /* if n ≡ 3 (mod 4) */
        res--;                       /* subtract 1 */
    return res;
}
