/*
 * A173431  Count of consecutive coprime iterations of sum-of-divisors
 *          function.
 * https://oeis.org/A173431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Starting from n, iterate the sum‑of‑divisors function σ.
   Count how many successive iterates are pairwise coprime until a
   non‑coprime pair occurs or a fixed point is reached.  */
#include <stdint.h>
#include "../utils/gcd.h"

static uint64_t sigma_uint64(uint64_t n)
/*  Compute σ(n) for a 64‑bit integer n using trial factorisation.  */
{
    if (n == 0) return 0;
    uint64_t result = 1;
    uint64_t temp = n;

    /* handle factor 2 */
    int exp = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        exp++;
    }
    if (exp > 0) {
        uint64_t term = 1;
        uint64_t cur   = 1;
        for (int i = 0; i < exp; ++i) {
            cur   *= 2;
            term += cur;
        }
        result *= term;
    }

    /* handle odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            uint64_t term = 1;
            uint64_t cur   = 1;
            for (int i = 0; i < exp; ++i) {
                cur   *= p;
                term += cur;
            }
            result *= term;
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        result *= (1 + temp);
    }

    return result;
}

seq_t a(idx_t n)
/* Return a(n): the length of the initial consecutive coprime
   σ‑iterates starting from n (with a(1)=1). */
{
    uint64_t val   = (uint64_t)n;
    seq_t   count = 1;                /* the first term always contributes */

    for (;;) {
        uint64_t next = sigma_uint64(val);

        if (next == val) break;                    /* fixed point reached */
        if (gcd((seq_t)val, (seq_t)next) != (seq_t)1) break;  /* non‑coprime */

        ++count;
        val = next;
    }
    return count;
}
