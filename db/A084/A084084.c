/*
 * A084084  Length of lists created by n substitutions k ->
 *          Range[0,1+Mod[k+1,3]] starting with {0}.
 * https://oeis.org/A084084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + a(n-3) with a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0)          return (seq_t)1;
    if (n == 1)          return (seq_t)3;
    if (n == 2)          return (seq_t)9;

    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)3;  /* a(n-2) */
    seq_t a2 = (seq_t)9;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 2*a2 + 3*a1 + a0; /* a(i)   */
        a0 = a1;                       /* shift window   */
        a1 = a2;
        a2 = next;
    }
    return a2;
}
