/*
 * A131067  Triangle read by rows: T(n,k) = 7*binomial(n,k) - 6 for 0 <= k <=
 *          n.
 * https://oeis.org/A131067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 7*binomial(r,k) - 6 where r is the row such that
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     u8 n <= 38, u16 n <= 142, u32 n <= 574, u64 n <= 2174,
 *            u128 n <= 5150, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute binomial coefficient C(n,k) using multiplicative formula with
   early cancellation to avoid overflow.  The result is compatible with
   arbitrary‑precision seq_t. */
static seq_t binom_with_cancellation(idx_t n, idx_t k)
{
    if (k > n) return 0;
    idx_t kk = k;
    if (kk > n - kk) kk = n - kk;   /* use symmetry */

    seq_t res = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        seq_t num = (seq_t)(n - kk + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd(num, den);    /* cancel common factors between num and den */
        num /= g; den /= g;

        g = gcd(res, den);          /* cancel remaining factors in den with current product */
        res /= g; den /= g;          /* den should now be 1 */

        /* multiply res by num using 128‑bit intermediate if available */
#ifdef __SIZEOF_INT128__
        unsigned __int128 tmp = (unsigned __int128)res * (unsigned __int128)num;
        res = (seq_t)tmp;
#else
        res *= num;
#endif
    }
    return res;
}

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        idx_t off = mid * (mid + 1) / 2;
        if (off <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t off_r = r * (r + 1) / 2;
    idx_t k = n - off_r;

    seq_t bin = binom_with_cancellation(r, k);

    /* compute 7*bin - 6 with 128‑bit precision if available */
    seq_t val;
#ifdef __SIZEOF_INT128__
    unsigned __int128 tmp = (unsigned __int128)bin * 7;
    val = (seq_t)tmp - (seq_t)6;
#else
    val = bin * 7 - 6;
#endif

    return val;
}
