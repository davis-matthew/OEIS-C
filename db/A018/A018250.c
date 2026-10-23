/*
 * A018250  Expansion of 1/((1-4x)(1-5x)(1-8x)).
 * https://oeis.org/A018250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 92*a(n-2) + 160*a(n-3); Initial values:
 *            a(0)=1, a(1)=17, a(2)=197
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)17;
    if (n == 2u) return (seq_t)197;

    /* Use only three variables to store the sliding window */
    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)17;  /* a(n-2)   */
    seq_t a2 = (seq_t)197; /* a(n-1)   */

    /* Iterate from 3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a3 = (seq_t)((seq_t)17 * a2 - (seq_t)92 * a1 + (seq_t)160 * a0);
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
