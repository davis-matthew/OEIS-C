/*
 * A040988  Primes p such that x^5 = 22 has a solution mod p.
 * https://oeis.org/A040988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 999 (2026-11-01)
 * Helpers:   gcd, is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040988: primes p such that x^5 ≡ 22 (mod p) has a solution.
   For a prime divisor of 22 (p = 2 or 11) the congruence is trivially solvable.
   For other primes the condition is equivalent to
     22^((p-1)/gcd(5,p-1)) ≡ 1 (mod p). */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static bool has_property(uint64_t p)
{
    /* 22 = 2 × 11; for p = 2 or 11 the equation is trivially solvable. */
    if (p == 2 || p == 11)
        return true;

    /* Test whether 22 is a 5th power residue modulo p. */
    seq_t d = gcd((seq_t)5, (seq_t)(p - 1));          /* d ∈ {1,5} */
    uint64_t exp = (p - 1) / (uint64_t)d;            /* exponent for the test */
    seq_t r = powmod((seq_t)22, exp, (seq_t)p);     /* 22^exp mod p */
    return r == (seq_t)1;
}

seq_t a(idx_t n)
{
    /* Cached list of primes satisfying the property. */
    static seq_t *good = NULL;          /* array of known primes   */
    static size_t good_cap = 0;         /* capacity of the array   */
    static size_t good_len = 0;         /* number of elements      */
    static uint64_t checked = 1;        /* last integer examined   */

    if (n <= good_len)
        return good[n - 1];

    /* Find more primes until we have at least n entries. */
    for (uint64_t p = checked + 1; good_len < (size_t)n; ++p) {
        if (!is_prime(p))
            continue;
        if (has_property(p)) {
            if (good_len == good_cap) {
                size_t new_cap = good_cap ? good_cap * 2 : 4;
                seq_t *tmp = (seq_t *)seq_realloc(good, good_cap, new_cap);
                good = tmp;
                good_cap = new_cap;
            }
            good[good_len++] = (seq_t)p;
        }
    }
    if (good_len > 0)
        checked = (uint64_t)good[good_len - 1];
    else
        checked = 1;   /* no primes found yet (should not happen) */

    return good[n - 1];
}
