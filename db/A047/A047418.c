/*
 * A047418  Numbers that are congruent to {0, 2, 3, 4, 6} mod 8.
 * https://oeis.org/A047418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n‑1)/5) + offs[(n‑1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offsets for remainders 0,1,2,3,4: 0,2,3,4,6 (a(1)=0, a(2)=2, a(3)=3, a(4)=4, a(5)=6) */

seq_t a(idx_t n)
{
    /* shift index so that n-1 starts at 0 */
    idx_t q = (n - 1) / 5;   /* number of complete 5‑step blocks */
    idx_t r = (n - 1) % 5;   /* remainder within the current block */

    /* constant offsets for each remainder class */
    static const seq_t offs[5] = { (seq_t)0, (seq_t)2, (seq_t)3, (seq_t)4, (seq_t)6 };

    /* compute a(n) */
    seq_t term = (seq_t)8 * (seq_t)q + offs[r];
    return term;
}
