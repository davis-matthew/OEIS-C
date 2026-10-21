/*
 * A216740  Primes p such that x^18 = -2 has no solution mod p.
 * https://oeis.org/A216740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, grow_u64, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216740: primes p such that x^18 = -2 has no solution mod p.
   The function builds the sequence on demand by generating primes
   and testing the 18th‐power condition using modular exponentiation.
   It caches results to allow out‑of‑order calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_u64.h"

/* Grow a dynamically allocated array of seq_t. */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* gcd for uint64_t, used in the 18th‑power test. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static cache of results and primes */
    static seq_t *found = NULL;          /* primes satisfying the condition */
    static size_t found_cap = 0, found_len = 0;
    static uint64_t *prime_list = NULL;  /* primes used for trial division */
    static size_t prime_cap = 0, prime_len = 0;
    static uint64_t last_checked = 1;    /* next candidate to test */

    if (n == 0) return 0;  /* out of range is ignored */

    /* Ensure we have started the prime list with 2 and 3 */
    if (prime_len == 0) {
        grow_u64(&prime_list, &prime_cap, 2);
        prime_list[0] = 2;
        prime_list[1] = 3;
        prime_len = 2;
    }

    /* Generate primes until we have enough satisfying primes */
    while (found_len < (size_t)n) {
        last_checked++;

        /* Skip even numbers greater than 2 */
        if (last_checked > 2 && (last_checked & 1ULL) == 0)
            continue;

        /* Primality test by trial division with previously found primes */
        uint64_t p = last_checked;
        bool prime = true;
        seq_t sqrtp = floor_sqrt((seq_t)p);
        uint64_t limit = (uint64_t)sqrtp;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t q = prime_list[i];
            if (q > limit)
                break;
            if (p % q == 0) { prime = false; break; }
        }
        if (!prime)
            continue;

        /* Store this prime for future trial divisions */
        grow_u64(&prime_list, &prime_cap, prime_len + 1);
        prime_list[prime_len++] = p;

        /* Skip the two trivial cases 2 and 3 */
        if (p == 2 || p == 3)
            continue;

        /* Test whether -2 is an 18th power modulo p.
           This holds iff (-2)^((p-1)/gcd(18,p-1)) ≡ 1 (mod p). */
        uint64_t g = gcd_u64(18ULL, p - 1ULL);
        uint64_t exp = (p - 1ULL) / g;
        seq_t base = (seq_t)(p - 2ULL);   /* -2 mod p for odd p */
        seq_t mod  = (seq_t)p;
        seq_t r = mod_pow(base, exp, mod);

        if (r != (seq_t)1) {
            /* This prime satisfies the condition; store it. */
            grow_array(&found, &found_cap, found_len + 1);
            found[found_len++] = (seq_t)p;
        }
    }

    return found[n - 1];
}
