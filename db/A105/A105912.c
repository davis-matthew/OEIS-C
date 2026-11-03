/*
 * A105912  Primes for which -40 is a primitive root.
 * https://oeis.org/A105912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime p for which -40 is a primitive root mod p
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 969 (2026-11-03)
 * Helpers:   is_prime_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function gathers qualifying primes lazily and returns the one
    with the requested index.  It keeps a static list so that calls
    are independent of call order.  The test for a primitive root is
    performed by checking that for every prime divisor q of p‑1,
    (-40)^( (p‑1)/q ) ≠ 1 (mod p).  Primes dividing 40 are skipped
    because (-40) ≡ 0 (mod p) and 0 cannot be a primitive root.    */

#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/powmod_u64.h"

/* Dynamically grow the array of found primes */
static void add_prime(uint64_t **arr, size_t *cap, size_t *cnt, uint64_t val)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(**arr));
        if (!tmp) return;           /* allocation failed – caller will free on exit */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*cnt)++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, return 0 */

    /* Static storage for primes that satisfy the condition */
    static uint64_t *prime_arr = NULL;
    static size_t   prime_cap = 0;
    static size_t   prime_cnt = 0;
    static uint64_t next_candidate = 3;    /* next odd number to test */

    /* Generate missing primes until we have enough */
    while (prime_cnt < (size_t)n) {
        /* Find the next prime candidate */
        while (!is_prime_u64(next_candidate))
            next_candidate += 2;           /* skip even numbers */

        /* Compute (-40 mod p) safely */
        uint64_t base = (next_candidate - (40 % next_candidate)) % next_candidate;
        if (base == 0) {                   /* p divides 40 → skip */
            next_candidate += 2;
            continue;
        }

        /* Test whether base is a primitive root modulo p */
        uint64_t m   = next_candidate - 1;
        uint64_t tmp = m;
        bool ok = true;

        /* Factor p‑1 and test each prime divisor of it */
        for (uint64_t d = 2; d * d <= tmp; ++d) {
            if (tmp % d == 0) {
                /* d is a prime factor */
                if (powmod_u64(base, m / d, next_candidate) == 1) {
                    ok = false;
                    break;
                }
                while (tmp % d == 0)
                    tmp /= d;
            }
        }
        if (ok && tmp > 1) {               /* remaining prime factor */
            if (powmod_u64(base, m / tmp, next_candidate) == 1)
                ok = false;
        }

        if (ok) {
            /* Store the qualifying prime */
            add_prime(&prime_arr, &prime_cap, &prime_cnt, next_candidate);
            /* Advance past the current prime for next search */
            next_candidate += 2;
        } else {
            /* Reject this prime, continue searching */
            next_candidate += 2;
        }
    }

    return (seq_t)prime_arr[(size_t)n - 1];
}
