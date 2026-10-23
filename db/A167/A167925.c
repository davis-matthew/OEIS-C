/*
 * A167925  Triangle, T(n, k) = (sqrt(k+1))^(n-1)*ChebyshevU(n-1,
 *          sqrt(k+1)/2), read by rows.
 * https://oeis.org/A167925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 33, i32 n <= 74, i64 n <= 168,
 *            i128 n <= 432, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle sequence A167925.
 * For n>0: a(n) is the element T(r,k) where
 *   r = smallest integer ≥1 such that (r-1)(r+2)/2 ≤ n-1 < r(r+3)/2,
 *   k = (n-1) - (r-1)(r+2)/2.
 * T(r,k) = (-1)^(r+1) * c_{r-1} with A = k+1,
 *   c_0 = 1,  c_1 = -A,
 *   c_m = -A * (c_{m-1} + c_{m-2})  for m ≥ 2.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* a(0) = 0 */

    idx_t pos = n - 1;                    /* index after the initial 0 */
    idx_t r = 1;                          /* current row number (≥1) */
    idx_t sum = 0;                        /* sum of lengths of rows < r */

    /* find row r such that sum <= pos < sum + (r+1) */
    while (sum + (r + 1) <= pos) {
        sum += r + 1;
        r++;
    }

    idx_t k = pos - sum;                  /* column within the row (0 ≤ k ≤ r) */
    seq_t A = (seq_t)(k + 1);              /* A = k+1 */

    if (r == 1)   /* c_0 = 1, T(1,k)=1 */
        return 1;

    seq_t c0 = 1;                          /* c_0 */
    seq_t c1 = -A;                         /* c_1 */

    if (r == 2)   /* T(2,k)=A */
        return (r & 1) ? c1 : -c1;         /* sign = (-1)^(r+1) */

    /* compute c_{r-1} using recurrence */
    for (idx_t m = 2; m < r; ++m) {
        seq_t cm = -(A) * (c1 + c0);       /* c_m = -A*(c_{m-1}+c_{m-2}) */
        c0 = c1;
        c1 = cm;
    }

    /* apply the parity sign: (-1)^(r+1) */
    return (r & 1) ? c1 : -c1;
}
