/*
 * A048155  a(n)=Sum{T(n,k): k=1,2,...,n}, array T as in A048154.
 * https://oeis.org/A048155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 – ∑_{k=1..n, n|k³} k
 * Range:     u8 n <= 25, u16 n <= 363, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;   /* sum 1..n */
    seq_t sub   = 0;                               /* subtract k with n|k³ */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 kk = k;
        /* compute k³ mod n using 128‑bit intermediates */
        unsigned __int128 c = (kk * kk % n) * kk % n;
        if (c == 0) {
            sub += (seq_t)k;
        }
    }
    return total - sub;
}
