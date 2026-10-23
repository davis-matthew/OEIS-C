/*
 * A030483  Cubes of primes, with property that all even digits occur together
 *          and all odd digits occur together.
 * https://oeis.org/A030483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 21, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   cube_parity_ok, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030483: Cubes of primes whose cube has all even digits together
 * and all odd digits together.
 * a(n) = (A030482(n))^3
 * where A030482(n) is the nth prime whose cube satisfies the parity block
 * property. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/cube_parity_ok.h"

/* Cached qualifying primes */
static seq_t *good_cache = NULL;
static size_t  good_cap   = 0;
static size_t  good_cnt   = 0;
/* Next prime index to examine (1‑based) */
static idx_t  prime_scan = 1;

/* Grow the dynamic array by at least needed elements. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed)
{
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* Expand the cache until it contains at least n qualifying primes. */
static void ensure_good_n(idx_t n)
{
    while (good_cnt < n)
    {
        seq_t p = get_prime(prime_scan);
        ++prime_scan;
        seq_t cube = p * p * p;
        if (cube_parity_ok(cube))
        {
            if (good_cnt == good_cap) grow_seq(&good_cache, &good_cap, good_cnt + 1);
            good_cache[good_cnt++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for offset 1 */
    ensure_good_n(n);
    seq_t p = good_cache[n - 1];
    return p * p * p;                  /* cube in seq_t */
}
