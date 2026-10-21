/*
 * A235800  Length of n-th vertical line segment from left to right in a
 *          diagram of a two-dimensional version of the 3x+1 (or Collatz)
 *          problem.
 * https://oeis.org/A235800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-4) for n > 4; a(1)=3, a(2)=1, a(3)=7,
 *            a(4)=2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for 0, return 0 */
    seq_t arr[4];
    arr[1] = 3;   /* a(1) */
    arr[2] = 1;   /* a(2) */
    arr[3] = 7;   /* a(3) */
    arr[0] = 2;   /* a(4) */

    if (n <= 4) return arr[n % 4];

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 2 * arr[(i-2) % 4] - arr[(i-4) % 4];
        arr[i % 4] = next;
    }
    return arr[n % 4];
}
