/*
 * A173120  Triangle T(n, k, q) = q*[n=2] + Sum_{j=0..5} q^j*binomial(n-2*j,
 *          k-j)*[n>2*j] with T(n,0) = T(n,n) = 1 for q = -4, read by rows.
 * https://oeis.org/A173120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, less
 * Formula:   a(n) gives the n‑th entry of the triangle T(n,k,q=-4) read by
 *            rows. The triangle is defined by T(n,k) = q*[n==2] + Σ_{j=0..5}
 *            q^j * binomial(n-2*j, k-j) * [n>2*j], with the boundary values
 *            T(n,0)=T(n,n)=1
 * Range:     i8 n <= 38, i16 n <= 215, i32 n <= 751, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include the provided binomial utility.  */
#include "../utils/binomial.h"

static const seq_t pow_q[6] = { (seq_t)1, (seq_t)-4, (seq_t)16,
                                (seq_t)-64, (seq_t)256, (seq_t)-1024 };

seq_t a(idx_t n)
{
    /* Determine row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t r = lo;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Handle the boundary values T(r,0)=T(r,r)=1. */
    if (k == 0 || k == r)
        return (seq_t)1;

    seq_t result = (seq_t)0;

    /* Special contribution when n==2. */
    if (r == (idx_t)2)
        result += (seq_t)-4;

    /* Main sum over j. */
    for (int j = 0; j <= 5; ++j) {
        if (r <= 2 * (idx_t)j)
            break;                /* r > 2*j condition fails */
        if (k < (idx_t)j)
            continue;            /* binomial would be zero */

        idx_t row_part = r - 2 * (idx_t)j;
        idx_t col_part = k - (idx_t)j;

        /* binomial(row_part, col_part) is zero when col_part > row_part, but
           the guard above ensures col_part ≤ row_part. */
        seq_t bin = binomial(row_part, col_part);
        result += pow_q[j] * bin;
    }

    return result;
}
