/*
 * A106150  6th diagonal of triangle in A059317.
 * https://oeis.org/A106150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/120)*(n-3)*(n^4 + 28*n^3 - 71*n^2 - 478*n + 1360) for
 *            n>3, a(n) = 0 for 0 ≤ n ≤ 3
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(0)=a(1)=a(2)=a(3)=0 */
    if (n < 4)
        return 0;

    /* Work with 128‑bit integers to avoid overflow in the intermediate computation. */
    unsigned __int128 u = n;
    unsigned __int128 n2 = u * u;        /* n^2 */
    unsigned __int128 n3 = n2 * u;       /* n^3 */
    unsigned __int128 n4 = n2 * n2;      /* n^4 */

    /* Compute the polynomial expression in signed 128‑bit to allow subtraction. */
    signed __int128 inner = (signed __int128)n4
                          + 28 * (signed __int128)n3
                          - 71 * (signed __int128)n2
                          - 478 * (signed __int128)u
                          + 1360;

    signed __int128 num = (signed __int128)(n - 3) * inner;
    num /= 120;                          /* exact division; the result is an integer */

    return (seq_t)num;
}
