/*
 * A156739  Triangle T(n, k, m) = round( Product_{j=0..m} binomial(2*(n+j),
 *          2*(k+j))/binomial( 2*(n-k+j), 2*j) ), where m = 6, read by rows.
 * https://oeis.org/A156739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k,6) where r and k are obtained by reading the
 *            triangle by rows, and T(r,k,6) = round( ∏_{j=0..6}
 *            C(2(r+j),2(k+j)) / C(2(r-k+j),2j) )
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 58, u128 n <= 141,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/gcd.h"

static inline seq_t
triangle_T(idx_t r, idx_t k)
{
    /* Compute the product ∏_{j=0..6} C(2(r+j),2(k+j)) /
     *                     C(2(r-k+j),2j)  as a rational num/den.
     * Then round the result to the nearest integer. */
    seq_t num = 1;   /* running numerator */
    seq_t den = 1;   /* running denominator */

    for (idx_t j = 0; j <= 6; ++j) {
        /* Numerator factor: C(2(r+j), 2(k+j)) */
        seq_t cur_num = binom(2 * (r + j), 2 * (k + j));

        /* Denominator factor: C(2(r-k+j), 2j) */
        seq_t cur_den = binom(2 * (r - k + j), 2 * j);

        /* Cancel common factors between the current numerator and
         * denominator first. */
        seq_t g = gcd(cur_num, cur_den);
        cur_num /= g;
        cur_den /= g;

        /* Reduce cur_den with the current running numerator. */
        g = gcd(num, cur_den);
        num /= g;
        cur_den /= g;

        /* Reduce cur_num with the current running denominator. */
        g = gcd(den, cur_num);
        den /= g;
        cur_num /= g;

        /* Multiply the reduced factors into the running product. */
        num *= cur_num;
        den *= cur_den;
    }

    /* Round the rational value num/den to the nearest integer. */
    seq_t q = num / den;
    seq_t rrem = num % den;
    /* If the remainder is at least half the denominator, round up. */
    if (rrem * 2 >= den)
        ++q;
    return q;
}

/* Map the linear index n to the corresponding row r and column k
 * of the triangle (row r has r+1 entries). */
seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    idx_t prev = (r * (r + 1)) / 2;
    idx_t k = n - prev;
    return triangle_T(r, k);
}
