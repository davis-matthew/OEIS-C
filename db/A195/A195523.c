/*
 * A195523  Number of lower triangles of a 5 X 5 -n..n array with all row and
 *          column sums zero.
 * https://oeis.org/A195523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) a(1)=199, a(2)=3753, a(3)=27267,
 *            a(4)=121367, a(5)=401565, a(6)=1089411, a(7)=2563933 Equivalent
 *            non‑negative polynomial: a(n) = (180 + 1314 n + 4423 n² + 8790
 *            n³ + 10825 n⁴ + 7716 n⁵ + 2572 n⁶) / 180 The division...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 175, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Quick lookup for the first seven terms */
    static const seq_t init[7] = {199, 3753, 27267, 121367, 401565, 1089411, 2563933};
    if (n <= 7) return init[n-1];

    /* Compute a(n) using the exact polynomial expression */
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;              /* n² */
    seq_t n3 = n2 * N;             /* n³ */
    seq_t n4 = n3 * N;             /* n⁴ */
    seq_t n5 = n4 * N;             /* n⁵ */
    seq_t n6 = n5 * N;             /* n⁶ */

    /* Numerator of (180 + 1314 n + 4423 n² + 8790 n³ + 10825 n⁴ + 7716 n⁵ + 2572 n⁶) */
    seq_t num = (seq_t)180
              + (seq_t)1314 * N
              + (seq_t)4423 * n2
              + (seq_t)8790 * n3
              + (seq_t)10825 * n4
              + (seq_t)7716 * n5
              + (seq_t)2572 * n6;

    return num / (seq_t)180;       /* exact division */
}
