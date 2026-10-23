/*
 * A048471  Array T read by diagonals: T(k,n) = 2^(k-1) * (3^n - 1) + 1.
 * https://oeis.org/A048471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the array * T(k,n) = 2^(k-1) * (3^n
 *            – 1) + 1 (k,n ≥ 0) * by diagonals starting with (k=0,n=0). For
 *            each * n, let s be the diagonal index (k+n = s) and * r = n –
 *            s(s+1)/2 be the position inside that diagonal. * Then k = r and
 *            n' = s – r; a(n) = T(k,n')
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find diagonal s and position r for index n */
    idx_t cum = 0;          /* number of terms before current diagonal */
    idx_t len = 1;          /* length of current diagonal (s+1) */
    while (cum + len <= n) {
        cum += len;
        len++;
    }
    idx_t s  = len - 1;     /* diagonal index */
    idx_t r  = n - cum;     /* position inside diagonal */
    idx_t k  = r;           /* k-coordinate */
    idx_t nprime = s - k;   /* n-coordinate for T(k,nprime) */

    /* Compute 3^nprime */
    seq_t p3 = 1;
    for (idx_t i = 0; i < nprime; ++i) {
        p3 *= (seq_t)3;
    }

    if (k == 0) {
        /* T(0,nprime) = (3^nprime + 1)/2 */
        return (p3 + (seq_t)1) / (seq_t)2;
    }

    /* Compute 2^k */
    seq_t p2k = 1;
    for (idx_t i = 0; i < k; ++i) {
        p2k *= (seq_t)2;
    }
    /* 2^(k-1) = p2k / 2 */
    seq_t p2kminus1 = p2k / (seq_t)2;

    /* T(k,nprime) = 2^(k-1) * (3^nprime – 1) + 1 */
    seq_t val = p3 - (seq_t)1;
    val = val * p2kminus1;
    val = val + (seq_t)1;
    return val;
}
