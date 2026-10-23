/*
 * A156725  Triangle T(n, k, m) = t(n,m)/( t(k,m) * t(n-k,m) ) with T(n, 0, m)
 *          = T(n, n, m) = 1, where t(n, m) = Product_{j=1..n}
 *          Product_{i=1..j-1} ( 1 - (m+1)*(3*i-2) ) and m = 2, read by rows.
 * https://oeis.org/A156725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th element of the triangle * T(n,k) defined by *
 *            T(n,0)=T(n,n)=1, * T(n,k)=t(n)/(t(k)*t(n-k)), * t(m)=
 *            ∏_{j=1..m}∏_{i=1..j-1}(1-(m+1)(3i-2)) * For the fixed m=2 this
 *            simplifies to * t(m)=∏_{i=1..m-1}(7-9i)^(m-i) . * The sequence
 *            is obtained by reading the triangle row‑wise * startin...
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 22, i64 n <= 38, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t prev = r * (r + 1) / 2;    /* number of elements before row r */
    idx_t k = n - prev;              /* column in row r (0 ≤ k ≤ r) */

    /* Endpoints of the triangle are always 1 */
    if (k == 0 || k == r)
        return 1;

    seq_t result = 1;

    for (idx_t i = 1; i < r; ++i) {
        /* Factor (7-9*i) which is negative for i≥1 */
        seq_t factor = (seq_t)7 - (seq_t)9 * (seq_t)i;

        /* Determine the exponent e_i for this factor */
        idx_t exp;
        if (i <= k - 1 && i <= r - k - 1) {
            exp = i;
        } else if (i <= k - 1) {
            exp = r - k;
        } else if (i <= r - k - 1) {
            exp = k;
        } else {
            exp = r - i;
        }

        /* Multiply factor^exp into the result */
        seq_t power = 1;
        for (idx_t t = 0; t < exp; ++t)
            power *= factor;
        result *= power;
    }

    return result;
}
