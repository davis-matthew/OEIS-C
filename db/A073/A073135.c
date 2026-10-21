/*
 * A073135  Table by antidiagonals of T(n,k) = 2*n*T(n,k-1) - n^2*T(n,k-2) +
 *          T(n,k-4) starting with T(n,1) = 1.
 * https://oeis.org/A073135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by traversing the table T(m,k) by
 *            anti‑diagonals. The table is defined for m ≥ 1 by T(m,1) = 1,
 *            T(m,2) = 2m, T(m,3) = 3m², T(m,4) = 4m³, and for k ≥ 5 T(m,k) =
 *            2*m*T(m,k-1) – m²*T(m,k-2) + T(m,k-4). The index n is 0‑based.
 *            Write n as n = T(d‑1) + offset with 0 ≤ offset < d, where...
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the antidiagonal number d such that T(d-1) ≤ n < T(d)   */
    idx_t d = 1;
    while ( ((seq_t)d * (d + 1) / 2) <= n ) ++d;

    idx_t tri_prev = (d - 1) * d / 2;   /* T(d-1)       */
    idx_t offset   = n - tri_prev;      /* position on diag  */
    idx_t m = offset + 1;               /* m = row index + 1   */
    idx_t k = d - offset;               /* column index         */

    /* compute T(m,k)                                           */
    seq_t m_seq = (seq_t)m;
    seq_t m2    = m_seq * m_seq;
    seq_t m3    = m2 * m_seq;

    switch (k) {
        case 1: return (seq_t)1;
        case 2: return (seq_t)2 * m_seq;
        case 3: return (seq_t)3 * m2;
        case 4: return (seq_t)4 * m3;
    }

    /* recurrence for k >= 5 */
    seq_t prev1 = (seq_t)1;      /* T(m, k-4)            */
    seq_t prev2 = (seq_t)2 * m_seq;  /* T(m, k-3)            */
    seq_t prev3 = (seq_t)3 * m2;     /* T(m, k-2)            */
    seq_t prev4 = (seq_t)4 * m3;     /* T(m, k-1)            */
    seq_t cur   = 0;

    for (idx_t i = 5; i <= k; ++i) {
        cur = (seq_t)2 * m_seq * prev4 - m2 * prev3 + prev1;
        prev1 = prev2;
        prev2 = prev3;
        prev3 = prev4;
        prev4 = cur;
    }
    return cur;
}
