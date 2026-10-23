/*
 * A141515  a(n) = phi(A067774(n)) where phi is Euler totient function.
 * https://oeis.org/A141515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(A067774(n)) where A067774(n) is the n-th prime p such
 *            that p + 2 is composite
 * Range:     u8 n <= 37, u16 n <= 5682, u32 n <= 6970, u64 n <= 6970,
 *            u128 n <= 6970, big any n
 * Helpers:   _floor_isqrt_u64, calc_phi, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/calc_phi.h"

static seq_t *a_cache      = NULL;   /* cached sequence values      */
static size_t  a_cache_cap  = 0;      /* capacity of a_cache         */
static size_t  a_cache_len  = 0;      /* number of terms stored      */

static uint64_t *prime_list = NULL;   /* list of discovered primes   */
static size_t    prime_list_cap  = 0;  /* capacity of prime_list      */
static size_t    prime_list_len  = 0;  /* number of primes stored     */
static uint64_t  candidate     = 2;    /* next integer to test        */

/* Grow an array of elements; returns the possibly reallocated pointer. */
static void *grow_array(void *ptr, size_t *cap, size_t *len, size_t elem_size)
{
    if (*len >= *cap) {
        size_t new_cap = *cap ? *cap * 2 : 16;
        void *tmp = realloc(ptr, new_cap * elem_size);
        if (!tmp) abort();              /* allocation failure */
        ptr = tmp;
        *cap = new_cap;
    }
    return ptr;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* Ensure the cache contains at least n terms. */
    while (a_cache_len < (size_t)n) {

        /* Find the next prime starting from 'candidate'. */
        while (true) {
            uint64_t x = candidate;
            bool is_prime = true;
            uint64_t limit = _floor_isqrt_u64(x);

            for (size_t i = 0; i < prime_list_len && prime_list[i] <= limit; ++i) {
                if (x % prime_list[i] == 0) { is_prime = false; break; }
            }

            if (is_prime) {
                /* Store the prime into the list. */
                prime_list = (uint64_t *)grow_array(
                        prime_list, &prime_list_cap, &prime_list_len,
                        sizeof(uint64_t));
                prime_list[prime_list_len++] = x;

                /* Check the condition p + 2 composite. */
                if (!is_prime_small((seq_t)(x + 2))) {
                    seq_t phi_val = calc_phi((seq_t)x);
                    a_cache = (seq_t *)grow_array(
                            a_cache, &a_cache_cap, &a_cache_len,
                            sizeof(seq_t));
                    a_cache[a_cache_len++] = phi_val;
                }

                /* Move to the next candidate. */
                ++candidate;
                break;
            }

            ++candidate;
        }
    }

    return a_cache[n - 1];
}
