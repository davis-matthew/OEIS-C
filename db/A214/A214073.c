/*
 * A214073  Least m>0 such that 2^n-m and n^2-m are relatively prime.
 * https://oeis.org/A214073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_uint64, mod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the least m>0 such that gcd(2^n - m, n^2 - m) == 1.
   For n^2 - m == 0 (i.e., m == n^2) the gcd equals |2^n - m|,
   so the special case is handled explicitly (only n = 1 or 3
   meet |2^n - n^2| == 1). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/mod_uint64.h"

/* Euclidean GCD for unsigned 64‑bit integers */

seq_t a(idx_t n)
{
    /* compute n^2 – fits in 64‑bit for all reasonable n */
    idx_t n_sq = n * n;                 /* n * n uses unsigned multiplication */

    for (idx_t m = 1; ; ++m) {
        /* |n^2 - m| – the modulus for the gcd test */
        idx_t absM = (m <= n_sq) ? (n_sq - m) : (m - n_sq);

        if (absM == 0) {                 /* m == n^2  →  gcd = |2^n - m| */
            /* |2^n - n^2| == 1 only for n = 1 or 3 */
            if (n == 1 || n == 3)
                return (seq_t)m;
            else
                continue;
        }

        uint64_t mod = absM;                 /* modulus for all further ops */

        uint64_t pow2mod = mod_uint64(2, n, mod);   /* 2^n mod mod */

        uint64_t m_mod = m % mod;
        /* (2^n - m) mod mod, avoiding negative values */
        uint64_t diff = (pow2mod >= m_mod) ?
                        (pow2mod - m_mod) :
                        (pow2mod + mod - m_mod);

        if (gcd_uint64(diff, mod) == 1)
            return (seq_t)m;
    }
}
