/*
 * A079504  a(n) = 8*n^3*((2*n-1)^2 - 4*n + 4).
 * https://oeis.org/A079504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3*((2*n-1)^2 - 4*n + 4)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t to avoid mixing types */
    seq_t nn  = (seq_t)n;          /* n               */
    seq_t n2  = nn * nn;           /* n²              */
    seq_t n3  = n2 * nn;           /* n³              */
    /* inner term: (2n-1)² - 4n + 4  → 4n² - 8n + 5 */
    seq_t inner = (seq_t)4 * n2 - (seq_t)8 * nn + (seq_t)5;
    seq_t term  = (seq_t)8 * n3;   /* 8·n³            */
    return term * inner;
}
