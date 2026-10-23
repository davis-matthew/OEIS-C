/*
 * A105900  Primes for which -28 is a primitive root.
 * https://oeis.org/A105900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, modpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the nth prime p such that the multiplicative order of (-28) modulo p
 *  equals p-1 (i.e., (-28) is a primitive root modulo p).  The function
 *  searches upward from 3, testing each odd prime number and collecting the
 *  ones that satisfy the primitive-root property.  The search result is
 *  cached so that subsequent calls for larger n are fast.  The implementation
 *  uses a simple trial‑division factorisation of p-1 and the modular
 *  exponentiation routine modpow_uint64.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(uint64_t n); */
#include "../utils/modpow_uint64.h"     /* uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod); */

static bool is_primitive_root(uint64_t p)
{
    if (p <= 2) return false;                   /* p=2 has trivial group */
    uint64_t base = p - (28ULL % p);
    base %= p;                                   /* base = (-28) mod p */
    if (base == 0) return false;                 /* p divides 28 */

    uint64_t orig = p - 1;
    uint64_t temp = orig;

    /* factor 2 */
    if ((temp & 1ULL) == 0) {
        if (modpow_uint64(base, orig / 2, p) == 1ULL) return false;
        while ((temp & 1ULL) == 0) temp >>= 1;
    }

    /* odd factors using 6k±1 optimisation */
    for (uint64_t q = 3; q * q <= temp; q += 2) {
        if (temp % q == 0) {
            if (modpow_uint64(base, orig / q, p) == 1ULL) return false;
            while (temp % q == 0) temp /= q;
        }
    }

    /* residual prime factor, if any */
    if (temp > 1) {
        if (modpow_uint64(base, orig / temp, p) == 1ULL) return false;
    }

    return true;
}

static seq_t *found = NULL;          /* cache of found primes                */
static idx_t found_len = 0;          /* number of primes stored in 'found'   */
static idx_t found_cap = 0;          /* allocated capacity                  */
static uint64_t current_candidate = 3;   /* next odd number to test            */

static void ensure_capacity(idx_t new_len)
{
    if (new_len <= found_cap) return;
    idx_t new_cap = found_cap ? found_cap * 2 : 16;
    if (new_cap < new_len) new_cap = new_len;
    seq_t *tmp = (seq_t *)realloc(found, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* allocation failed */
    found = tmp;
    found_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for offset 1 */

    /* expand stored list until it contains at least n primes   */
    while (found_len < n) {
        if (is_prime(current_candidate)) {
            if (is_primitive_root(current_candidate)) {
                ensure_capacity(found_len + 1);
                found[found_len++] = (seq_t)current_candidate;
            }
        }
        current_candidate += 2;             /* skip even numbers          */
    }
    return found[n - 1];
}
