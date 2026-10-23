/*
 * A155989  List of numbers prime(k) as k runs through the numbers with a
 *          single prime digit.
 * https://oeis.org/A155989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000040(A092620(n))
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto, is_single_prime_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the n-th prime whose index is the n-th integer containing a single prime digit */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_single_prime_digit.h"

/* cache for the sequence of numbers that contain a single prime digit */
static uint64_t *k_cache = NULL;          /* k_cache[i] = A092620(i+1) */
static size_t k_cache_cap = 0;
static idx_t   k_cache_len = 0;
static uint64_t next_candidate = 1;        /* next number to test */

/* ensure the cache covers at least n elements */
static uint64_t get_kth_with_one_prime_digit(idx_t n)
{
    if (n <= k_cache_len)
        return k_cache[n - 1];

    /* enlarge the cache until we have at least n entries */
    while (k_cache_len < n) {
        /* test numbers until we find the next qualifying one */
        do {
            ++next_candidate;
        } while (!is_single_prime_digit(next_candidate));

        /* grow the array if necessary */
        if (k_cache_len == k_cache_cap) {
            size_t new_cap = (k_cache_cap == 0) ? 64 : k_cache_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(k_cache, new_cap * sizeof(uint64_t));
            if (!tmp) abort();                      /* OOM */
            k_cache = tmp;
            k_cache_cap = new_cap;
        }
        k_cache[k_cache_len++] = next_candidate;
    }
    return k_cache[n - 1];
}

/* cache for primes */
static uint64_t *prime_list = NULL;        /* prime_list[i] = p_{i+1} */
static idx_t     prime_list_len = 0;

/* simple upper bound for the n-th prime: p_n <= 15 * n for n >= 1 */
static idx_t prime_upper_bound(idx_t n)
{
    if (n < 6)
        return 15;
    return n * 15;
}

/* ensure we have at least k primes in prime_list */
static void ensure_primes_up_to(idx_t k)
{
    if (prime_list_len >= k)
        return;

    idx_t bound = prime_upper_bound(k);
    while (1) {
        free(prime_list);
        build_primes_upto(bound, &prime_list, &prime_list_len);
        if (prime_list_len >= k)
            break;
        bound *= 2;            /* increase bound if not enough primes */
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    uint64_t k = get_kth_with_one_prime_digit(n);     /* A092620(n) */
    ensure_primes_up_to(k);                          /* build primes up to p_k */
    return (seq_t)prime_list[k - 1];                  /* A000040(k) */
}
