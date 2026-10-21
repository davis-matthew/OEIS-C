/*
 * A063379  Number of orbits of the group of units of Z/(n) acting naturally
 *          on the 2-subsets of Z/(n).
 * https://oeis.org/A063379
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of orbits of the group of units of Z/nZ acting on
 *            the 2‑subsets of Z/nZ. Computed via Burnside's lemma
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean gcd on unsigned integers. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* a(n): number of orbits of U(n) on 2‑subsets of Z/nZ. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* values below the offset are irrelevant */

    unsigned __int128 sumC  = 0;          /* sum of C(Fix(u),2) over all units u */
    unsigned __int128 sumF  = 0;          /* sum of Fix(u) over all units u */
    unsigned __int128 sumF2 = 0;          /* sum of Fix(u²) over all units u */
    idx_t phi = 0;                        /* |U(n)| */

    for (idx_t u = 1; u < n; ++u) {
        if (gcd_idx(u, n) != 1) continue;   /* skip non‑units */
        ++phi;

        idx_t d1 = gcd_idx(u - 1, n);      /* Fix(u) = gcd(u-1,n)   */
        idx_t d2 = gcd_idx((u * u) - 1, n);/* Fix(u²) = gcd(u²-1,n) */

        sumC  += (unsigned __int128)d1 * (d1 - 1) / 2;
        sumF  += d1;
        sumF2 += d2;
    }

    unsigned __int128 numerator = sumC + (sumF2 - sumF) / 2;
    seq_t result = (seq_t)(numerator / (unsigned __int128)phi);
    return result;
}
