/*
 * A207302  Number of n X 5 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*n^4+49*n^3+16*n^2-43*n+9)/3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 170, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 13, a(2) = 169, a(3) = 665, a(4) = 1759, a(5) = 3773 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */
    seq_t N  = (seq_t)n;           /* cast to sequence type */
    seq_t n2 = N * N;              /* n^2 */
    seq_t n3 = n2 * N;             /* n^3 */
    seq_t n4 = n3 * N;             /* n^4 */
    seq_t num = 8 * n4 + 49 * n3 + 16 * n2 - 43 * N + 9;
    return num / (seq_t)3;
}
