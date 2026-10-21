/*
 * A228161  Number triangle associated to Chebyshev polynomials of the second
 *          kind.
 * https://oeis.org/A228161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   * a(n) = U_{r-k}(k), where * r = floor((sqrt(8*n+1)-1)/2), * k =
 *            n - r*(r+1)/2, * and U_m(x) is the Chebyshev polynomial of the
 *            second kind * (U_0(x)=1, U_1(x)=2x,
 *            U_{m+1}(x)=2x*U_m(x)-U_{m-1}(x)). * Thus the triangle is given
 *            by T(r,k)=U_{r-k}(k) and a(n) is its * row‑by‑row enumeration
 * Range:     i8 n <= 22, i16 n <= 47, i32 n <= 123, i64 n <= 305,
 *            i128 n <= 827, big any n
 * Helpers:   chebyshev_U
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/chebyshev_U.h"

/* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
static idx_t find_row(idx_t n)
{
    /* initial estimate using double arithmetic                               */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);

    /* adjust with 128‑bit arithmetic so no overflow occurs                 */
    while (1) {
        unsigned __int128 start = (unsigned __int128)r * (r + 1) / 2;
        if (start > n) {
            --r;
            continue;
        }
        unsigned __int128 next = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (next <= n) {
            ++r;
            continue;
        }
        break;
    }
    return r;
}

/* Main required function                                               */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;   /* column index   */
    idx_t m = r - k;       /* order of U   */
    return chebyshev_U(m, (seq_t)k);
}
