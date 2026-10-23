/*
 * A112620  If b(n,1) = n; b(n,m) is number of terms among {b(n,1), b(n,2),
 *          ..., b(n,m-1)} which are coprime to m, then a(n) = b(n,n).
 * https://oeis.org/A112620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by evaluating the definition of b(n,m):
   b(n,1)=n; for m>1, b(n,m) = |{k<m : gcd(b(n,k),m)=1}|;
   then a(n)=b(n,n)                                          */

#include <stdlib.h>
#include <stdint.h>

/* Euclidean algorithm for seq_t */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS offset */
    /* allocate array b[1..n] (index 0 unused) */
    seq_t *b = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!b) return 0;                      /* allocation failure (unlikely) */

    b[1] = (seq_t)n;                          /* b(n,1) = n */

    for (idx_t m = 2; m <= n; ++m) {
        seq_t cnt = 0;
        for (idx_t k = 1; k < m; ++k) {
            if (gcd_seq(b[k], (seq_t)m) == (seq_t)1) {
                ++cnt;
            }
        }
        b[m] = cnt;
    }

    seq_t result = b[n];
    free(b);
    return result;
}
