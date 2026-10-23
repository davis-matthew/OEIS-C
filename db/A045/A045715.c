/*
 * A045715  Primes with first digit 9.
 * https://oeis.org/A045715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 127, u32 n <= 125, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Computes the n‑th prime whose first decimal digit is 9 (OEIS A045715).
   a(1) = 97, a(2) = 907, …  */
static int first_digit(idx_t x)
{
    while (x >= 10) x /= 10;
    return (int)x;
}

/* Lazy storage for primes that start with the digit 9. */
static seq_t *candidates = NULL;   /* array of primes */
static size_t cand_len   = 0;      /* number of stored primes */
static size_t cand_cap   = 0;      /* allocated capacity */
static idx_t  search     = 2;      /* next integer to test */

/* Ensure that at least n candidates are stored. */
static void ensure_candidates(idx_t n)
{
    while (cand_len < n)
    {
        /* Move to the next integer whose most significant digit is 9. */
        while (first_digit(search) != 9) ++search;

        /* Test primality using the provided routine. */
        if (is_prime_big((seq_t)search))
        {
            /* Grow the array if required. */
            if (cand_len >= cand_cap)
            {
                size_t new_cap = (cand_cap == 0) ? 16 : cand_cap * 2;
                seq_t *tmp = (seq_t *)realloc(candidates, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* fatal allocation error */
                candidates = tmp;
                cand_cap = new_cap;
            }
            candidates[cand_len++] = (seq_t)search;
        }
        ++search;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1 – undefined for n = 0 */
    ensure_candidates(n);
    return candidates[n - 1];         /* a(1) is the first stored prime */
}
