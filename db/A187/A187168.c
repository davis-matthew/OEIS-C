/*
 * A187168  Number of 7-step self-avoiding walks on an n X n X n cube summed
 *          over all starting positions.
 * https://oeis.org/A187168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = 16926*n^3 - 85380*n^2 + 128832*n - 57312 for n>5. Initial
 *            values: a(1) = 0 a(2) = 192 a(3) = 25344 a(4) = 177120 a(5) =
 *            568248 For n <= 5 the table is used; for n >= 6 the polynomial
 *            is applied
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small-table for n <= 5 */
    static const seq_t tbl[5] = {
        (seq_t)0,          /* a(1) */
        (seq_t)192,        /* a(2) */
        (seq_t)25344,      /* a(3) */
        (seq_t)177120,     /* a(4) */
        (seq_t)568248      /* a(5) */
    };

    if (n <= 5) {
        return tbl[n - 1];
    }

    /* Compute N = n, N2 = n^2, N3 = n^3 using seq_t */
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;     /* n^2 */
    seq_t N3 = N2 * N;    /* n^3 */

    /* Apply the closed‑form polynomial */
    seq_t term1 = (seq_t)16926 * N3;   /* 16926*n^3 */
    seq_t term2 = (seq_t)85380 * N2;   /* 85380*n^2 */
    seq_t term3 = (seq_t)128832 * N;   /* 128832*n  */
    seq_t term4 = (seq_t)57312;        /* constant  */

    seq_t val = term1 - term2 + term3 - term4;
    return val;
}
