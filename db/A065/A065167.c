/*
 * A065167  Table T(n,k) read by antidiagonals, where the k-th row gives the
 *          permutation t->t+k of Z, folded to N (k >= 0, n >= 1).
 * https://oeis.org/A065167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = f(g(t)+k) where k = n - d(d+1)/2, t = d - k + 1, d =
 *            floor((sqrt(8*n+1)-1)/2), f(z) = 2*z (z>0) else 2*|z|+1, g(n) =
 *            n/2 (n even) else (1-n)/2. (The sequence is read by
 *            antidiagonals of the table T(k,t)=f(g(t)+k) with t≥1.)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External definition as required by the build system. */
seq_t a(idx_t n)
{
    /* Determine the diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2. */
    idx_t d = 0;
    idx_t sum = 0;                     /* total entries before current diagonal */
    while (sum + d + 1 <= n) {
        sum += d + 1;
        ++d;
    }

    /* Position k inside the diagonal (0‑based). */
    idx_t k = n - sum;                 /* 0 ≤ k ≤ d */

    /* Column index t (starts from 1). */
    idx_t t = d - k + 1;               /* t = (d-k) + 1 */

    /* Compute g(t).  t≥1. */
    int64_t g_val;
    if ((t & 1ULL) == 0) {             /* t even */
        g_val = (int64_t)(t >> 1);
    } else {                           /* t odd */
        g_val = ((int64_t)1 - (int64_t)t) / 2;
    }

    /* z = k + g(t). */
    int64_t z = (int64_t)k + g_val;

    /* Apply f to z. */
    seq_t result;
    if (z > 0) {
        result = (seq_t)(2 * (seq_t)z);
    } else {
        seq_t absz = (seq_t)(-z);      /* |z| as unsigned */
        result = 2 * absz + 1;
    }
    return result;
}
