/*
 * A141822  Maximum term in the continued fraction of A141821(n)/n.
 * https://oeis.org/A141822
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) – minimal maximum partial quotient over reduced fractions k/n
   (1 ≤ k < n, gcd(k,n)=1).  Offset: 2 (a(2) is the first term). */
#include <stdint.h>

/* Euclidean algorithm for unsigned 64‑bit integers. */
static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* undefined for n < 2 */

    idx_t best = n;                       /* worst case: maximum partial quotient could be n */
    for (idx_t k = 1; k < n; ++k) {
        if (gcd_idx_t(k, n) != 1) continue;   /* skip non‑coprime k */

        idx_t local_max = 0;
        idx_t a1 = n, b1 = k;

        while (b1 != 0) {
            idx_t q = a1 / b1;
            if (q > local_max) {
                local_max = q;
                if (local_max >= best) break;   /* cannot improve this k */
            }
            idx_t r = a1 % b1;
            a1 = b1;
            b1 = r;
        }

        if (local_max < best) {
            best = local_max;
            if (best == 2) break;              /* minimal possible value reached */
        }
    }
    return (seq_t)best;
}
