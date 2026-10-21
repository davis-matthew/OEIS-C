/*
 * A073458  a(n) = floor(prime(n)/composite(n)).
 * https://oeis.org/A073458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( prime(n) / composite(n) )
 * Range:     u8 n <= 54, u16 n <= 6642, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence element a(n): floor of the ratio of the nth prime to the nth composite. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

/* Static state used to cache the first prime and composite numbers. */
static seq_t *prime_arr      = NULL;   /* array of first primes */
static seq_t *composite_arr  = NULL;   /* array of first composites (starting at 4) */
static size_t prime_count    = 0;      /* how many primes we have stored */
static size_t composite_count = 0;     /* how many composites we have stored */
static size_t prime_cap      = 0;      /* capacity of prime_arr */
static size_t composite_cap  = 0;      /* capacity of composite_arr */
static idx_t  next_num       = 2;      /* next integer to test */

/* Ensure that prime_arr and composite_arr have at least 'need' elements. */
static void ensure_capacity(size_t need, size_t *cap, seq_t **arr)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? need * 2 : (*cap) * 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *arr = tmp;
    *cap = new_cap;
}

/* Fill the tables until we have at least 'need' primes and composites. */
static void fill_until(size_t need)
{
    while (prime_count < need || composite_count < need) {
        if (is_prime(next_num)) {
            /* prime */
            ensure_capacity(prime_count + 1, &prime_cap, &prime_arr);
            prime_arr[prime_count++] = (seq_t)next_num;
        } else if (next_num > 3) {     /* composites start at 4 */
            ensure_capacity(composite_count + 1, &composite_cap, &composite_arr);
            composite_arr[composite_count++] = (seq_t)next_num;
        }
        ++next_num;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, but return 0 */
    fill_until((size_t)n);
    seq_t p = prime_arr[(size_t)n - 1];
    seq_t c = composite_arr[(size_t)n - 1];
    return p / c;                  /* integer division = floor for positive numbers */
}
