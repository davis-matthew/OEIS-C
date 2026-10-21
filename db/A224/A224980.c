/*
 * A224980  Number of primes of the form p-q-1 where q is any prime < p =
 *          prime(n).
 * https://oeis.org/A224980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of primes of the form p − q − 1, where q is any prime < p
 *            = prime(n)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

/* helpers */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list  = NULL;   /* list of primes up to current limit */
static size_t   prime_count = 0;       /* number of primes stored */
static uint8_t *is_prime_arr = NULL;   /* boolean table for primality up to limit */
static idx_t    last_limit  = 0;       /* maximum value of the last sieve */

/* Ensure that primes up to at least the n‑th prime are available. */
static void
ensure_primes_up_to(idx_t n)
{
    idx_t needed_limit = _prime_limit_for_idx(n);
    if (needed_limit <= last_limit)
        return;   /* already sufficient */

    /* free previous data */
    free(prime_list);
    free(is_prime_arr);
    prime_list = NULL;
    prime_count = 0;
    is_prime_arr = NULL;
    last_limit = 0;

    /* build primes up to the new limit */
    build_primes_upto(needed_limit, &prime_list, &prime_count);

    /* create primality table */
    size_t sz = (size_t)needed_limit + 1;
    is_prime_arr = (uint8_t*)malloc(sz);
    if (!is_prime_arr)
        abort();        /* allocation failed */
    memset(is_prime_arr, 0, sz);
    for (size_t i = 0; i < prime_count; ++i)
        is_prime_arr[prime_list[i]] = 1;

    last_limit = needed_limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset starts at 1 */

    ensure_primes_up_to(n);

    uint64_t p = prime_list[n - 1];   /* nth prime, offset 1 */
    seq_t    count = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q >= p)
            break;                 /* only primes less than p */
        uint64_t r = p - q - 1;
        if (r >= 2 && is_prime_arr[(size_t)r])
            ++count;
    }

    return count;
}
