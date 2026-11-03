/*
 * A056284  Number of n-bead necklaces with exactly four different colored
 *          beads.
 * https://oeis.org/A056284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) ∑_{d|n} φ(d)·(4^{n/d} – 4·3^{n/d} + 6·2^{n/d} – 4)
 *            (number of n‑bead necklaces with exactly four different colored
 *            beads, turning over not allowed). The implementation uses a
 *            128‑bit intermediate when seq_t is a small native unsigned
 *            integer, and the arbitrary‑precision utilities oth...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 969 (2026-11-01)
 * Helpers:   ipow_i32_idx, pow2, pow3, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32_idx.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/totient.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety: offset starts at 1 */

    /* Decide whether seq_t is a small native type or a big‑integer type. */
    if (sizeof(seq_t) <= 8)                  /* small type: use 128‑bit intermediate */
    {
        unsigned __int128 sum = 0;

        for (idx_t d = 1; d * d <= n; ++d)
        {
            if (n % d != 0) continue;

            /* first divisor d */
            idx_t m1 = n / d;
            __int128 p2  = ipow_i32_idx(2, m1);
            __int128 p3  = ipow_i32_idx(3, m1);
            __int128 p4  = ipow_i32_idx(4, m1);
            __int128 part = p4 - (__int128)4 * p3 + (__int128)6 * p2 - (__int128)4;
            sum += (unsigned __int128)totient(d) * (unsigned __int128)part;

            /* second divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                idx_t m2 = n / d2;
                __int128 p2b = ipow_i32_idx(2, m2);
                __int128 p3b = ipow_i32_idx(3, m2);
                __int128 p4b = ipow_i32_idx(4, m2);
                __int128 partb = p4b - (__int128)4 * p3b + (__int128)6 * p2b - (__int128)4;
                sum += (unsigned __int128)totient(d2) * (unsigned __int128)partb;
            }
        }

        return (seq_t)(sum / (unsigned __int128)n);
    }
    else /* big-integer path */
    {
        seq_t sum = 0;

        for (idx_t d = 1; d * d <= n; ++d)
        {
            if (n % d != 0) continue;

            /* first divisor d */
            idx_t m1 = n / d;
            seq_t part = pow4(m1);
            seq_t t3 = pow3(m1);
            part -= (seq_t)4 * t3;
            seq_t t2 = pow2(m1);
            part += (seq_t)6 * t2;
            part -= (seq_t)4;
            sum += totient(d) * part;

            /* second divisor n/d, if distinct */
            idx_t d2 = n / d;
            if (d2 != d)
            {
                idx_t m2 = n / d2;
                seq_t partb = pow4(m2);
                seq_t t3b = pow3(m2);
                partb -= (seq_t)4 * t3b;
                seq_t t2b = pow2(m2);
                partb += (seq_t)6 * t2b;
                partb -= (seq_t)4;
                sum += totient(d2) * partb;
            }
        }

        return sum / n;
    }
}
