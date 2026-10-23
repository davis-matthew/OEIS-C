/*
 * A143080  Triangular sequence of coefficients from an exponential based
 *          polynomial: p(x,n)=If[n == 0, 1, -(2*n - 1)!*x^(2*n)*(Sum[x^i/(i +
 *          2*n)!, {i, 0, Infinity}] - Exp[x]/x^(2*n))].
 * https://oeis.org/A143080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 43, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A143080
   It enumerates the coefficients of the polynomial p(x,n).
   For row r (r≥2) the coefficients are
        a = (2r-3)! / k! ,  k = 0 .. 2r-3
   The sequence is laid out row‑by‑row, starting with row 1.
   The function below computes a(n) for any n ≥ 1 by determining the
   row r and the local index k and then multiplying the integers from
   k+1 up to 2r‑3.  This uses only integer operations and works for the
   native unsigned integer types as well as for big‑integer builds. */
seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n == 1u) return (seq_t)1;

    /* Find the row r such that r(r‑1)+1 ≥ n */
    idx_t r = 1;
    while ((r * (r - 1) + 1) < n) ++r;

    /* Number of terms preceding this row */
    idx_t pre = (r - 1) * (r - 2) + 1;
    /* Position within the row (1‑based) */
    idx_t offset = n - pre;          /* 1 .. 2r‑2 */
    if (r <= 2u) {                   /* rows 1 and 2 contain only 1’s */
        return (seq_t)1;
    }

    /* Compute the product (k+1)*(k+2)*…*(2r‑3),
       where k = offset‑1 (0‑based) */
    idx_t k     = offset - 1;            /* 0 .. 2r‑3 */
    idx_t limit = 2 * r - 3;             /* upper bound of the product */
    seq_t result = (seq_t)1;

    for (idx_t i = k + 1; i <= limit; ++i) {
        result *= (seq_t)i;
    }
    return result;
}
