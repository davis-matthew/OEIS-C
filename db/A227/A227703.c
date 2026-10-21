/*
 * A227703  The Wiener index of the zig-zag polyhex nanotube TUHC_6[2n,2]
 *          defined pictorially in Fig. 1 of the Eliasi et al. reference.
 * https://oeis.org/A227703
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(1 + 2*n + 2*n^2)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 0.  */
    if (n < 2) return 0;

    seq_t ns = (seq_t)n;            /* n as seq_t */
    seq_t n2 = ns * ns;             /* n^2 */

    /* Compute 1 + 2*n + 2*n^2 */
    seq_t term = 2 * ns;            /* 2*n */
    term += 2 * n2;                 /* + 2*n^2 */
    term += 1;                      /* + 1 */

    /* Return 2*n * term */
    return 2 * ns * term;
}
