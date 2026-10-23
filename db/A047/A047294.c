/*
 * A047294  Numbers that are congruent to {1, 2, 4, 6} mod 7.
 * https://oeis.org/A047294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = numbers ≡ {1,2,4,6} (mod 7) a(n) = 7*floor((n-1)/4) +
 *            offset[(n-1)%4] offset[0]=1, offset[1]=2, offset[2]=4,
 *            offset[3]=6
 * Range:     u8 n <= 146, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1-indexed: n>=1, behaviour for n==0 is undefined but handled */
    if (n == 0) return 0;
    idx_t q = (n - 1) / 4;          /* block number   */
    unsigned r = (unsigned)((n - 1) % 4);  /* position in block 0..3 */

    /* offsets for the 4 admissible residues in each 7-block */
    static const seq_t offs[4] = { (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)6 };

    /* compute result: 7*q + offs[r] */
    seq_t res = ((seq_t)q) * (seq_t)7 + offs[r];
    return res;
}
