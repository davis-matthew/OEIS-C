/*
 * A198711  Number of n X 4 0..3 arrays with values 0..3 introduced in row
 *          major order and no element equal to any horizontal or vertical
 *          neighbor.
 * https://oeis.org/A198711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 46*a(n-1) - 312*a(n-2) + 530*a(n-3) - 263*a(n-4) with
 *            initial values a(1)=5, a(2)=172, a(3)=6548, a(4)=250031
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return 5;
    if (n == 2) return 172;
    if (n == 3) return 6548;
    if (n == 4) return 250031;

    /* v0 = a(n-4), v1 = a(n-3), v2 = a(n-2), v3 = a(n-1) */
    seq_t v0 = 5,     /* a(1)   */
          v1 = 172,   /* a(2)   */
          v2 = 6548,  /* a(3)   */
          v3 = 250031;/* a(4)   */
    seq_t curr;

    for (idx_t i = 5; i <= n; ++i) {
        curr = 46 * v3 - 312 * v2 + 530 * v1 - 263 * v0;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = curr;
    }
    return v3; /* a(n) */
}
