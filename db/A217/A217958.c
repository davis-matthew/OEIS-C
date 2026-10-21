/*
 * A217958  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..2 n X 2 array.
 * https://oeis.org/A217958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - a(n-4); Initial values: a(1)=3,
 *            a(2)=17, a(3)=77, a(4)=343
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, offset is 1 */
    if (n == 1) return 3;
    if (n == 2) return 17;
    if (n == 3) return 77;
    if (n == 4) return 343;

    /* we only need the last four values to compute the next one */
    seq_t a1 = 343; /* a(n-1)  */
    seq_t a2 = 77;  /* a(n-2)  */
    seq_t a3 = 17;  /* a(n-3)  */
    seq_t a4 = 3;   /* a(n-4)  */
    seq_t cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        cur = 4 * a1 + 2 * a2 - a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = cur;
    }
    return cur;
}
