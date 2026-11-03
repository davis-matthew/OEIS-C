/*
 * A163776  a(n) is the n-th dS-prime (dual Shuffle prime).
 * https://oeis.org/A163776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A163781(n) (the n-th dS-prime)
 * Range:     u8 n <= 21, u16 n <= 2466, u32 n <= 3612, u64 n <= 3612,
 *            u128 n <= 3612, big any n
 * Validated: standard, pass, 71 terms through n = 3473 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

/* Returns the n-th dS-prime (dual Shuffle prime) a(n). */
seq_t a(idx_t n)
{
    idx_t found = 0;

    for (idx_t p = 3; ; p += 2) {
        if (!is_prime(p))
            continue;                      /* only odd primes are candidates */

        /* test whether -2 is a primitive root modulo p */
        idx_t phi = p - 1;
        idx_t temp = phi;
        bool ok = true;

        /* factor 2 from p-1 */
        if ((temp & 1) == 0) {
            idx_t exp = phi >> 1;          /* phi / 2 */
            if (powmod((seq_t)(p - 2), exp, (seq_t)p) == 1)
                ok = false;
            while ((temp & 1) == 0)
                temp >>= 1;
        }
        if (!ok)
            continue;

        /* factor odd primes */
        for (idx_t d = 3; d <= temp / d; d += 2) {
            if (temp % d == 0) {
                idx_t exp = phi / d;
                if (powmod((seq_t)(p - 2), exp, (seq_t)p) == 1) {
                    ok = false;
                    break;
                }
                while (temp % d == 0)
                    temp /= d;
            }
        }
        if (!ok)
            continue;

        /* remaining prime factor > 1 */
        if (temp > 1) {
            idx_t exp = phi / temp;
            if (powmod((seq_t)(p - 2), exp, (seq_t)p) == 1)
                ok = false;
        }
        if (!ok)
            continue;

        /* p is a qualifying prime, so N = p - 1 is a dS-prime */
        found++;
        if (found == n)
            return (seq_t)(p - 1);
    }
}
