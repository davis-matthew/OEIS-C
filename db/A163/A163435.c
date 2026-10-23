/*
 * A163435  Number of different fixed (possibly) disconnected pentominoes
 *          bounded tightly by an n X n square.
 * https://oeis.org/A163435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/3)*n^2*(n-2)^2*(5*n^2-10*n+2) for n>1, a(1)=0, a(2)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 34, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2   = nn * nn;            /* n^2 */
    seq_t nm2  = nn - 2;             /* n-2 */
    seq_t nm22 = nm2 * nm2;          /* (n-2)^2 */
    seq_t coeff = 5 * nn * nn - 10 * nn + 2; /* 5*n^2-10*n+2 */

    unsigned __int128 num = (unsigned __int128)n2 * (unsigned __int128)nm22;
    num = num * (unsigned __int128)coeff; /* multiply by coeff */
    num = num / 3;    /* divide by 3 */
    num = num * 2;    /* multiply by 2 */

    return (seq_t)num;
}
