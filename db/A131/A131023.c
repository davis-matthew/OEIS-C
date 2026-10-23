/*
 * A131023  First subdiagonal of triangular array T: T(j,1) = 1 for ((j-1) mod
 *          6) < 3, else 0; T(j,k) = T(j-1,k-1) + T(j-1,k) for 2 <= k <= j.
 * https://oeis.org/A131023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=3, a(4)=4; for n>4 : a(n)=5*a(n-1) -
 *            9*a(n-2) + 6*a(n-3)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* offset starts at 1, behaviour below 1 is undefined */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)4;

    seq_t a1 = (seq_t)1; /* a(1)   */
    seq_t a2 = (seq_t)2; /* a(2)   */
    seq_t a3 = (seq_t)3; /* a(3)   */
    seq_t a4 = (seq_t)4; /* a(4)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)5 * a4 - (seq_t)9 * a3 + (seq_t)6 * a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
