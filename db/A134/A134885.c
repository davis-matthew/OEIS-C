/*
 * A134885  Triangle reads by rows: T(n,k) = [x^k] p(n,x) where p(0,x)=1,
 *          p(1,x)=137-x, and p(n,x)=(-1)^(n-1)*(137-n) +
 *          (-1)^(n-1)*137*x^(n-1) - (-1)^(n-1)*x^n.
 * https://oeis.org/A134885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, less, dumb
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,k) where r = ceil( (sqrt(8n+1)-1)/2 )
 *                 k = n - r(r-1)/2
 * The triangular array T(r,k) is defined by the polynomials
 *
 *   p(0,x) = 1,
 *   p(1,x) = 137 - x,
 *   p(r,x) = (-1)^(r-1)*(137 - r) + (-1)^(r-1)*137*x^(r-1) - (-1)^(r-1)*x^r
 *
 * For r = 1, T(1,1) = 1.
 * For r = 2,   T(2,1) = 137,  T(2,2) = -1.
 * For r >= 3:
 *   T(r,1)   = (-1)^r * (138 - r),
 *   T(r,r-1) = (-1)^r * 137,
 *   T(r,r)   = (-1)^(r-1),
 *   all other T(r,k) = 0.
 *
 * The sequence a(n) enumerates T(r,k) row‑wise: a(1)=T(1,1),
 * a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 * 
 * This implementation follows the definition directly and uses
 * only integer arithmetic.
 */

seq_t a(idx_t n)
{
    /* Locate the row r containing the n-th term.
     * Use a simple incremental search because n is small (≤10000).   */
    idx_t r = 0;
    idx_t cum = 0;         /* cumulative count of terms up to current row */
    while (cum < n) {
        ++r;
        cum += r;          /* add number of terms in this row */
    }

    /* Index k (1‑based) within the row r. */
    idx_t k = n - (cum - r);   /* cum - r is the number of terms preceding row r */

    /* Special handling for the first two rows. */
    if (r == 1) {
        return (seq_t)1;
    }
    if (r == 2) {
        return (k == 1) ? (seq_t)137 : (seq_t)-1;
    }

    /* r >= 3 */
    seq_t sign = (r & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^r */

    if (k == 1) {                       /* coefficient of x^0 in p(r,x) */
        return sign * ((seq_t)138 - (seq_t)r);
    }
    if (k == r - 1) {                   /* coefficient of x^(r-2) */
        return sign * (seq_t)137;
    }
    if (k == r) {                       /* coefficient of x^(r-1) */
        return ((r - 1) & 1) ? (seq_t)-1 : (seq_t)1;
    }

    return (seq_t)0;                     /* all other entries are zero */
}
