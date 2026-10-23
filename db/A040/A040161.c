/*
 * A040161  Primes p such that x^5 = 3 has a solution mod p.
 * https://oeis.org/A040161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that x^5 ≡ 3 (mod p) has a solution.
   Computed by incrementally testing primes. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

/* Check whether a prime p satisfies the solvability condition. */
static bool good_prime(uint64_t p)
{
    /* p = 2 is special – x = 1 works. */
    if (p == 2) return true;

    /* If p ≢ 1 (mod 5), gcd(5, p-1) = 1 and the congruence
       automatically holds. */
    if ((p - 1) % 5 != 0) return true;

    /* For p ≡ 1 (mod 5) we need 3^((p-1)/5) ≡ 1 (mod p). */
    uint64_t exp = (p - 1) / 5;
    seq_t r = mod_pow((seq_t)3, exp, (seq_t)p);
    return r == (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1 – unused value */

    /* Static storage for the found primes. */
    static seq_t *good = NULL;
    static size_t good_cap = 0;
    static size_t good_len = 0;
    static uint64_t last = 2;   /* next candidate to test */

    /* Continue searching until we have the n‑th prime. */
    while (good_len < n)
    {
        for (uint64_t p = last; ; ++p)
        {
            if (!is_prime(p)) continue;

            if (good_prime(p))
            {
                /* Grow the array if needed. */
                if (good_len == good_cap)
                {
                    size_t new_cap = good_cap == 0 ? 8 : good_cap * 2;
                    seq_t *tmp = (seq_t*)realloc(good, new_cap * sizeof(seq_t));
                    if (!tmp) return 0;          /* allocation failed */
                    good = tmp;
                    good_cap = new_cap;
                }
                good[good_len++] = (seq_t)p;

                if (good_len == n)
                {
                    last = p + 1;
                    break;                  /* enough primes collected */
                }
            }
        }
    }

    return good[n - 1];   /* a(1) is the first element */
}
