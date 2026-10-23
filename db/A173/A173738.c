/*
 * A173738  Square array read by antidiagonals (n >= 0, k >= 2): T(n,k) =
 *          b(n,k) + b(k-2,n+2), where b(n,k) = ((1 + sqrt(k))^n + (1 -
 *          sqrt(k))^n)/2.
 * https://oeis.org/A173738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = b(pos, k) + b(k-2, pos+2) where pos = n - d(d+1)/2 k = d
 *            - pos + 2 d = floor((sqrt(8*n+1)-1)/2) and b(n,k) = ((1 +
 *            sqrt(k))^n + (1 - sqrt(k))^n)/2
 * Range:     u8 n <= 35, u16 n <= 91, u32 n <= 277, u64 n <= 824,
 *            u128 n <= 1274, big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_one.h"

/* Compute b(n,k) = ((1+√k)^n + (1-√k)^n)/2
   using the recurrence   b(0)=1, b(1)=1,
   b(i)=2*b(i-1)+(k-1)*b(i-2).                      */
static seq_t compute_b(idx_t n, idx_t k)
{
    seq_t one = seq_one();
    if (n == 0) return one;

    seq_t a = one;                 /* b(0,k) */
    seq_t b = one;                 /* b(1,k) */
    seq_t k_minus1 = (seq_t)(k - 1);

    for (idx_t i = 2; i <= n; ++i) {
        seq_t t1 = b + b;          /* 2*b(i-1) */
        seq_t t2 = a * k_minus1;   /* (k-1)*b(i-2) */
        seq_t c  = t1 + t2;        /* b(i)   */
        a = b;
        b = c;
    }
    return b;                      /* b(n,k) */
}

seq_t a(idx_t n)
{
    /* Find the diagonal d such that
       (d+1)(d+2)/2 > n  and  d(d+1)/2 <= n.  */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) {
        ++d;
    }

    idx_t terms_before = d * (d + 1) / 2;   /* count of terms before diagonal d */
    idx_t pos          = n - terms_before;  /* position within diagonal d (0..d) */

    idx_t n_val = pos;                      /* n value in T(n,k) */
    idx_t k_val = d - pos + 2;              /* k value in T(n,k), k >= 2 */

    seq_t b1 = compute_b(n_val, k_val);
    seq_t b2 = compute_b(k_val - 2, n_val + 2);
    return b1 + b2;
}
