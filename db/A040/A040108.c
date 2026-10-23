/*
 * A040108  Primes p such that x^4 = 6 has no solution mod p.
 * https://oeis.org/A040108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by iteratively testing primes for absence of a fourth‑root
   of 6 modulo the prime.  A prime p is accepted if 6^{(p−1)/gcd(4,p−1)}
   mod p ≠ 1.  The function lazily generates primes until it has found
   at least n accepted primes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"
#include "../utils/append_candidate.h"

/* --- internal helpers ---------------------------------------------- */

static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- static state --------------------------------------------------- */

static seq_t *accepted = NULL;      /* array of primes that satisfy the property */
static size_t accepted_len = 0;
static size_t accepted_cap = 0;

static uint64_t last_checked = 1;   /* greatest integer that has been examined for primality */

/* Ensure we have at least target primes stored in accepted[] */
static void ensure_size(idx_t target)
{
    while (accepted_len < (size_t)target) {
        /* find next prime after last_checked */
        uint64_t cand = last_checked + 1;
        for (;;) {
            if (is_prime_big((seq_t)cand)) {
                break;
            }
            cand += 1;
        }

        /* test whether 6 has a fourth power modulo cand */
        if (cand > 3) {
            uint64_t g = gcd_uint64(4, cand - 1);
            uint64_t exp = (cand - 1) / g;
            seq_t chk = mod_pow((seq_t)6, exp, (seq_t)cand);
            if (chk != (seq_t)1) {
                append_candidate(&accepted, &accepted_len, &accepted_cap, (seq_t)cand);
            }
        }

        last_checked = cand;
    }
}

/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    ensure_size(n);
    return accepted[(size_t)n - 1];
}
