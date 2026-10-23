/*
 * A171232  Array read by antidiagonals, T(n,k) = 2*(n/k) - 1, if n mod k = 0;
 *          otherwise, T(n,k) = 1.
 * https://oeis.org/A171232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn, tabl
 * Formula:   a(n) = T(i,j) read by antidiagonals, where T(n,k) = 2*(n/k)-1 if
 *            n % k == 0, otherwise 1
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#ifdef __GNUC__
# ifdef __SIZEOF_INT128__
#  define HAVE_INT128 1
# endif
#endif

seq_t a(idx_t n)
{
    /* Handle trivial n (usually unused) */
    if (n == 0)
        return (seq_t)0;

    /* --- determine diagonal d such that d*(d+1)/2 >= n --- */
    double d_f = sqrt((double)8 * n + 1.0);
    idx_t d = (idx_t)((d_f - 1.0) / 2.0);      /* floor value */

    /* Adjust up or down to satisfy triangle condition */
    while (((unsigned __int128)d * (d + 1) / 2) < n)
        ++d;
    while (d > 0 && (((unsigned __int128)(d - 1) * d / 2) >= n))
        --d;

    /* --- compute previous triangular number ---
       prev_tri = (d-1)*d/2   */
    unsigned __int128 prev_big = (unsigned __int128)(d - 1) * d / 2;
    idx_t prev_tri = (idx_t)prev_big;

    /* Position inside diagonal (0‑based) */
    idx_t pos = n - prev_tri - 1;

    /* Coordinates i (row) and j (column) */
    idx_t i = d - pos;
    idx_t j = pos + 1;

    /* Compute T(i,j) */
    if (i % j == 0) {
        idx_t q = i / j;
        seq_t val = (seq_t)(2 * q - 1);
        return val;
    } else {
        return (seq_t)1;
    }
}
