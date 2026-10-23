/*
 * A019400  Primes with primitive root 77.
 * https://oeis.org/A019400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cap_uint, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 77 is a primitive root modulo p.
   The function memorises all discovered primes and the subset that
   satisfy the primitive‑root condition, so that any order of calls
   to a(idx_t n) is answered quickly. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_cap_uint.h"

/* static storage for all primes discovered so far */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t  prime_cap  = 0;
static size_t  prime_len  = 0;

/* static storage for primes with 77 as a primitive root */
static seq_t *good_list = NULL;       /* array of good primes */
static size_t good_cap  = 0;
static size_t good_len  = 0;

static uint64_t last_checked = 1;     /* last integer examined as prime */

/* ensure the |arr| array has at least |need| elements */
static void ensure_cap_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* test whether 77 is a primitive root modulo the prime p */
static bool is_primitive_root_77(uint64_t p)
{
    /* 77 must be coprime to p */
    if ((p % 7ULL) == 0ULL || (p % 11ULL) == 0ULL) return false;

    uint64_t rem = p - 1;

    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > rem) break;
        if (rem % q == 0) {
            if (mod_pow((seq_t)77, (idx_t)((p - 1) / q), (seq_t)p) == (seq_t)1)
                return false;
            while (rem % q == 0) rem /= q;
        }
    }

    if (rem > 1) {
        if (mod_pow((seq_t)77, (idx_t)((p - 1) / rem), (seq_t)p) == (seq_t)1)
            return false;
    }

    return true;
}

/* external function required by the build */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n==0 at this offset */

    /* keep generating primes until we have at least n good primes */
    while (good_len < (size_t)n) {
        /* scan for the next prime */
        for (++last_checked; ; ++last_checked) {
            if (is_prime(last_checked)) {
                /* store prime in the list */
                ensure_cap_uint(&prime_list, &prime_cap, prime_len + 1);
                prime_list[prime_len++] = last_checked;

                /* check primitive‑root condition */
                if (is_primitive_root_77(last_checked)) {
                    ensure_cap_seq(&good_list, &good_cap, good_len + 1);
                    good_list[good_len++] = (seq_t)last_checked;
                    break;   /* found one more good prime, try to reach n */
                }
            }
        }
    }

    return good_list[n - 1];
}
