/*
 * A216107  The hyper-Wiener index of the tetrameric 1,3-adamantane TA(n) (see
 *          the Fath-Tabar et al. reference).
 * https://oeis.org/A216107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (75n^4 +210n^3 + 229n^2-178n)/2
 * Range:     u8 none, u16 n <= 4, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the hyper-Wiener index of the tetrameric 1,3-adamantane TA(n) */
/* Offset: 1  (a(1) is the first term listed) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, safe guard */
    seq_t N = (seq_t)n;

    /* Compute powers of n safely */
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n3 * N;         /* n^4 */

    /* Build numerator */
    seq_t num = 75 * n4;               /* 75 n^4 */
    num += 210 * n3;                   /* + 210 n^3 */
    num += 229 * n2;                   /* + 229 n^2 */
    num -= 178 * N;                    /* - 178 n */

    /* Divide by 2 (exact division) */
    return num / 2;
}
