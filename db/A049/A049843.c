/*
 * A049843  Triangular array T read by rows: T(n,k)=number of nonzero
 *          remainders when Euclidean algorithm acts on primes prime(n) and
 *          prime(k), k=1,2,...,n-1; n=2,3,4,...
 * https://oeis.org/A049843
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049843: triangular array T(n,k) = number of nonzero remainders
   in the Euclidean algorithm applied to the primes prime(n) and prime(k)
   (k = 1,2,…,n‑1).  The sequence a(n) is this array read by rows,
   starting at n = 2. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;          /* flattened array values */
static size_t cache_size = 0;        /* number of cached terms (a(2)… ) */
static size_t cache_cap  = 0;        /* allocated capacity for cache */
static idx_t  next_row   = 2;        /* next row number to compute */

static void
append_value(seq_t val)
{
    if (cache_size >= cache_cap) {
        cache_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = val;
}

static void
compute_next_row(void)
{
    uint64_t prime_n = get_prime(next_row);   /* prime(n) for this row */

    for (idx_t k = 1; k < next_row; ++k) {
        uint64_t a = prime_n;
        uint64_t b = get_prime(k);
        seq_t cnt = 0;

        /* Euclidean algorithm – count non‑zero remainders */
        while (b != 0) {
            uint64_t rem = a % b;
            a = b;
            b = rem;
            if (rem != 0)
                ++cnt;
        }

        append_value(cnt);
    }

    ++next_row;
}

static void
ensure_terms(idx_t n)
{
    /* cache_size counts terms starting with a(2) at index 0 */
    while (cache_size < (size_t)(n - 1))
        compute_next_row();
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;

    ensure_terms(n);
    return cache[n - 2];
}
