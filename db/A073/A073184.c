/*
 * A073184  Number of cubefree divisors of n.
 * https://oeis.org/A073184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (min(e,2)+1) Number of cubefree divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------ */

#include <stdint.h>

/* Count the cubefree divisors of n (n >= 1).  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of range – value irrelevant */

    uint64_t nn = (uint64_t)n;    /* use 64‑bit arithmetic for factorisation */
    seq_t   res = 1;
    uint64_t cnt;

    /* handle factor 2 separately */
    if ((nn & 1ULL) == 0) {
        cnt = 0;
        while ((nn & 1ULL) == 0) {
            nn >>= 1;
            ++cnt;
        }
        /* factor contribution: e = cnt
           min(e,2)+1  →  if e<2 : e+1 ; else : 3   */
        res *= (cnt >= 2 ? 3 : (cnt + 1));
    }

    /* odd prime factors */
    for (uint64_t p = 3; p <= nn / p; p += 2) {
        if (nn % p == 0) {
            cnt = 0;
            while (nn % p == 0) {
                nn /= p;
                ++cnt;
            }
            res *= (cnt >= 2 ? 3 : (cnt + 1));
        }
    }

    /* remaining prime factor (if any) */
    if (nn > 1) {
        /* exponent 1 → factor 2 */
        res *= 2;
    }

    return res;
}
