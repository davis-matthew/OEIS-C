/*
 * A023053  Number of noncrossing rooted trees with n nodes on a circle that
 *          do not have leaves at level 1.
 * https://oeis.org/A023053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023053: Number of noncrossing rooted trees with n nodes on a circle that do not have leaves at level 1.
   Recurrence (valid for n ≥ 4):

       2*n*(2*n+1)*a(n)
     + (-47*n^2+65*n-24)*a(n-1)
     + 3*(49*n^2-167*n+148)*a(n-2)
     + (-65*n^2+365*n-396)*a(n-3)
     - 12*(3*n-5)*(3*n-7)*a(n-4) = 0.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values (offset 0) */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)7;

    /* previous four terms a(n‑4), a(n‑3), a(n‑2), a(n‑1) */
    seq_t a0 = (seq_t)1;  /* a(0) */
    seq_t a1 = (seq_t)0;  /* a(1) */
    seq_t a2 = (seq_t)2;  /* a(2) */
    seq_t a3 = (seq_t)7;  /* a(3) */
    seq_t ak = 0;         /* current a(n) */

    /* choose a calculation strategy depending on the size of seq_t */
    if (sizeof(seq_t) <= 8) {
        /* seq_t is 64‑bit (or smaller) – use 128‑bit accumulation */
        for (idx_t k = 4; k <= n; ++k) {
            unsigned long long kk = (unsigned long long)k;

            unsigned long long coeff1 = 47ULL * kk * kk - 65ULL * kk + 24ULL;
            unsigned long long coeff2 = 3ULL * (49ULL * kk * kk - 167ULL * kk + 148ULL);
            long long       coeff3  = 65LL * kk * kk - 365LL * kk + 396LL;
            unsigned long long coeff4 = 12ULL * (3ULL * kk - 5ULL) * (3ULL * kk - 7ULL);

            unsigned __int128 pos = 0, neg = 0;

            /* a(n-1) term – always positive */
            pos += (unsigned __int128)a3 * coeff1;

            /* a(n-2) term – always negative */
            neg += (unsigned __int128)a2 * coeff2;

            /* a(n-3) term – sign depends on coeff3 */
            if (coeff3 >= 0)
                pos += (unsigned __int128)a1 * (unsigned long long)coeff3;
            else
                neg += (unsigned __int128)a1 * (unsigned long long)(-coeff3);

            /* a(n-4) term – always positive on the RHS */
            pos += (unsigned __int128)a0 * coeff4;

            unsigned __int128 numerator = pos >= neg ? pos - neg : neg - pos;
            unsigned long long denom = 2ULL * kk * (2ULL * kk + 1ULL);

            ak = (seq_t)(numerator / denom);

            /* rotate the window for the next iteration */
            a0 = a1;
            a1 = a2;
            a2 = a3;
            a3 = ak;
        }
    } else {
        /* seq_t is a big‑integer type – use its native operations */
        for (idx_t k = 4; k <= n; ++k) {
            unsigned long long kk = (unsigned long long)k;

            unsigned long long coeff1 = 47ULL * kk * kk - 65ULL * kk + 24ULL;
            unsigned long long coeff2 = 3ULL * (49ULL * kk * kk - 167ULL * kk + 148ULL);
            long long       coeff3  = 65LL * kk * kk - 365LL * kk + 396LL;
            unsigned long long coeff4 = 12ULL * (3ULL * kk - 5ULL) * (3ULL * kk - 7ULL);

            seq_t pos = 0, neg = 0;

            pos += a3 * (seq_t)coeff1;
            neg += a2 * (seq_t)coeff2;

            if (coeff3 >= 0)
                pos += a1 * (seq_t)coeff3;
            else
                neg += a1 * (seq_t)(-coeff3);

            pos += a0 * (seq_t)coeff4;

            seq_t numerator = (pos >= neg) ? (pos - neg) : (neg - pos);
            seq_t denom = (seq_t)(2ULL * kk * (2ULL * kk + 1ULL));

            ak = numerator / denom;

            a0 = a1; a1 = a2; a2 = a3; a3 = ak;
        }
    }
    return ak;
}
