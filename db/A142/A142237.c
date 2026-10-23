/*
 * A142237  Primes congruent to 40 mod 41.
 * https://oeis.org/A142237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   compute the n-th prime congruent to 40 modulo 41 by iterating
 *            over candidates of the form 40+41*k and testing primality
 * Range:     u8 n <= 1, u16 n <= 162, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/ensure_capacity.h"

static seq_t *ap_arr = NULL;    /* array of stored primes ≡ 40 (mod 41) */
static size_t ap_cap = 0;       /* allocated capacity of ap_arr */
static size_t ap_len = 0;       /* number of stored primes */
static idx_t last_candidate = 40; /* last checked candidate (starts just below first check) */

/* Ensure that ap_arr contains at least n such primes */
static void ensure_cache(idx_t n)
{
    while (ap_len < n) {
        /* Find the next prime of the required congruence class */
        do {
            last_candidate += 41ULL;
            /* Skip if the new candidate is <= 41 (not prime) */
        } while (!is_prime(last_candidate));

        /* Grow the array if necessary */
        if (ap_len == ap_cap) {
            size_t new_cap = (ap_cap == 0) ? 8 : ap_cap * 2;
            ensure_capacity(ap_len + 1, &ap_cap, (void **)&ap_arr, sizeof(seq_t));
        }

        ap_arr[ap_len++] = (seq_t)last_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n=0; return 0 as a safe default */

    ensure_cache(n);
    return ap_arr[n - 1];
}
