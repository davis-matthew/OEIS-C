/*
 * A075114  Perfect powers k such that 2*k + 1 is a perfect power; the value
 *          of y^b in the solution of the Diophantine equation x^a - 2y^b = 1.
 * https://oeis.org/A075114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   Let y₁ = 2, y₂ = 12, y_{k+1} = 6*y_k – y_{k-1} (k ≥ 1). The
 *            sequence a(n) is defined by a(1) = 4 ( = 2² ) a(2) = 121 ( =
 *            11², special term ) a(n) = y_{n-2}² for n ≥ 3 This enumerates
 *            the squares y² for which 2*y² + 1 is a perfect power (xᵃ),
 *            providing the terms of OEIS A075114
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* outside offset – undefined */
    if (n == 1) return (seq_t)4;           /* 2²  */
    if (n == 2) return (seq_t)121;         /* 11² special */

    idx_t k = n - 2;                       /* position in Pell sequence, starting with y₂ = 12 */
    seq_t prev = (seq_t)2;                 /* y₁ */
    seq_t curr = (seq_t)12;                /* y₂ */

    for (idx_t i = 1; i < k; ++i) {
        seq_t next = (seq_t)6 * curr - prev;
        prev = curr;
        curr = next;
    }

    return curr * curr;                    /* square of the Pell y value */
}
