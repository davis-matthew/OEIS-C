/*
 * A120070  Triangle of numbers used to compute the frequencies of the
 *          spectral lines of the hydrogen atom.
 * https://oeis.org/A120070
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   The sequence is the row‑wise enumeration of the triangle a(m,n)
 *            = m^2 - n^2 for 2 ≤ m , 1 ≤ n ≤ m-1. The offset is 2, so a(2) is
 *            the first term (m=2,n=1)
 * Range:     u8 n <= 121, u16 n <= 9428, u32 n <= 9428, u64 n <= 9428,
 *            u128 n <= 9428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term (n ≥ 2) of A120070.  The function works
   for any idx_t and seq_t.  It uses only integer arithmetic. */
seq_t a(idx_t n)
{
    /* Index 0 is below the offset – its value is irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* Zero‑based index within the flattened triangle. */
    idx_t p = n - 2;

    /* Find the row m such that (m-1)*m/2 > p. */
    idx_t m = 2;
    while ((m - 1) < p + 1)            /* (m-1)*m/2 > p  ⇔  m-1 > p/(m)  */
    {                                  /* simple linear search, fast for n≤10 000 */
        if (((m - 1) * m) / 2 > p)
            break;
        ++m;
    }

    /* Total number of terms preceding row m.  */
    idx_t prev = (m - 2) * (m - 1) / 2;

    /* Zero‑based position inside row m.  */
    idx_t pos = p - prev;

    /* One‑based column index within the row.  */
    seq_t r = (seq_t)(pos + 1);

    /* m, r are now converted to seq_t for the final calculation.  */
    seq_t m_sq = (seq_t)m * (seq_t)m;
    seq_t r_sq = r * r;

    return m_sq - r_sq;
}
