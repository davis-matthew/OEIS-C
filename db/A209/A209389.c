/*
 * A209389  Product of positive odd integers smaller than n and relatively
 *          prime to n, taken Modd n. A209388(n) (Modd n).
 * https://oeis.org/A209389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A209389: a(n) = ∏_{1≤k<n, k odd, gcd(k,n)=1} k  (Modd n) */
#include <stdint.h>
#include <stdbool.h>

/* Euclidean algorithm for idx_t (uint64_t). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Compute the product modulo 2*n.  The result k = P mod (2*n) gives
       both the reduced residue r = k mod n and the parity of the
       quotient floor(P/n) via comparison k>=n. */
    unsigned __int128 M = (unsigned __int128)n << 1;   /* 2*n  */
    unsigned __int128 k = 1;                         /* product mod M */

    for (idx_t i = 1; i < n; i += 2) {               /* odd i < n */
        if (gcd_idx(i, n) == 1) {
            k = (k * (unsigned __int128)i) % M;
        }
    }

    bool odd = k >= (unsigned __int128)n;
    unsigned __int128 r = k % (unsigned __int128)n;

    unsigned __int128 ans;
    if (odd) {
        /* Negate residue modulo n.  (n - r) % n works even for r==0. */
        ans = (unsigned __int128)n - r;
        ans %= (unsigned __int128)n;
    } else {
        ans = r;
    }

    return (seq_t)ans;
}
