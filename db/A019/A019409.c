/*
 * A019409  Primes with primitive root 87.
 * https://oeis.org/A019409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that 87 is a primitive root modulo p (search)
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seqt, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seqt.h"

/* grow a dynamic array of uint64_t */
static void grow_uint64(uint64_t **arr, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
        *cap = newcap;
    }
    (*len)++;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t  *valid   = NULL;  /* cache of primes with the property */
    static size_t valid_len   = 0, valid_cap   = 0;
    static uint64_t *primes   = NULL;  /* list of all primes seen so far */
    static size_t primes_len   = 0, primes_cap   = 0;
    static uint64_t last_checked = 1;
    static bool initialized = false;

    /* initialise once */
    if (!initialized) {
        valid_cap = 16;
        valid = (seq_t *)malloc(valid_cap * sizeof(seq_t));
        valid_len = 0;
        primes_cap = 16;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        primes_len = 0;
        initialized = true;
    }

    /* extend cache until we have at least n terms */
    while (valid_len < (size_t)n) {
        /* next candidate */
        last_checked++;

        if (!is_prime((idx_t)last_checked)) {
            /* not a prime, but we still keep it for future factorisation */
            if (is_prime((idx_t)last_checked)) a(0); /* dummy to keep types consistent */
            continue;
        }

        /* add to the prime list for future factorisations */
        grow_uint64(&primes, &primes_len, &primes_cap);
        primes[primes_len - 1] = last_checked;

        /* skip primes dividing 87 */
        if (last_checked == 3 || last_checked == 29) continue;

        uint64_t rem = last_checked - 1;
        bool ok = true;
        for (size_t i = 0; i < primes_len && primes[i] * primes[i] <= rem; ++i) {
            uint64_t q = primes[i];
            if (rem % q == 0) {
                /* test 87^( (p-1)/q ) mod p != 1 */
                seq_t r = mod_pow((seq_t)87, ((uint64_t)(last_checked - 1)) / q, (seq_t)last_checked);
                if (r == 1) {
                    ok = false;
                    break;
                }
                while (rem % q == 0) rem /= q;
            }
        }

        if (ok && rem > 1) {
            seq_t r = mod_pow((seq_t)87, ((uint64_t)(last_checked - 1)) / rem, (seq_t)last_checked);
            if (r == 1) ok = false;
        }

        if (ok) {
            grow_seqt(&valid, &valid_len, &valid_cap);
            valid[valid_len - 1] = (seq_t)last_checked;
        }
    }

    return valid[n - 1];
}
