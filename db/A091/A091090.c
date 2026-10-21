/*
 * A091090  In binary representation: number of editing steps (delete, insert,
 *          or substitute) to transform n into n + 1.
 * https://oeis.org/A091090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; a(1)=1; a(2*n)=1; a(2*n+1)=a(n)+1 (n>0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    if ((n & (idx_t)1) == 0)   /* even n -> 2*n case */
        return (seq_t)1;
    return a(n >> 1) + (seq_t)1;   /* odd n -> 2*n+1 case */
}
