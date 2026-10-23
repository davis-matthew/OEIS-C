/*
 * A154508  Numbers k such that appending k to the k-th nonprime yields a
 *          prime.
 * https://oeis.org/A154508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 13, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_len, is_prime_big, is_prime_small, pow10, seq_alloc,
 *            seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154508: k such that concatenation of the k‑th nonprime and k is prime.
   a(n) returns the n‑th such k (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* below offset – behaviour irrelevant */

    /* --------- cache for sequence values --------- */
    static seq_t *seq_cache = NULL;
    static size_t seq_cap  = 0;
    static size_t seq_len  = 0;

    if (seq_cache == NULL)
    {
        seq_cache = seq_alloc(n);
        seq_cap   = n;
        seq_len   = 0;
    }
    else if (seq_cap < n)
    {
        seq_cache = seq_realloc(seq_cache, seq_cap, n);
        seq_cap   = n;
    }

    /* --------- cache for non‑prime numbers --------- */
    static seq_t *nonprime_cache = NULL;
    static size_t nonprime_cap   = 0;
    static size_t nonprime_len   = 0;
    static uint64_t cur_val      = 0;   /* next integer to test for non‑prime */

    /* --------- k values to examine --------- */
    static idx_t next_k = 1;

    /* Generate sequence values until we have n of them */
    while (seq_len < n)
    {
        /* Make sure we have the next_k‑th nonprime available */
        while (nonprime_len < next_k)
        {
            /* Grow the nonprime cache if necessary */
            if (nonprime_cap == 0)
            {
                nonprime_cap   = 16;
                nonprime_cache = seq_alloc(nonprime_cap);
            }
            else if (nonprime_len == nonprime_cap)
            {
                size_t new_cap   = nonprime_cap * 2;
                nonprime_cache   = seq_realloc(nonprime_cache, nonprime_cap, new_cap);
                nonprime_cap     = new_cap;
            }

            /* Find the next nonprime */
            while (is_prime_small((seq_t)cur_val))
                cur_val++;                 /* skip primes */

            nonprime_cache[nonprime_len++] = cur_val;
            cur_val++;                     /* advance for next candidate */
        }

        /* Retrieve the next_k‑th nonprime */
        seq_t np = nonprime_cache[next_k - 1];

        /* Compute concatenation np || next_k */
        int dk = digit_len((seq_t)next_k);                 /* number of decimal digits of next_k */
        seq_t pow10d = pow10((idx_t)dk);
        seq_t concat  = np * pow10d + (seq_t)next_k;

        /* Test primality of the concatenated number */
        if (is_prime_big(concat))
            seq_cache[seq_len++] = next_k;

        ++next_k;
    }

    return seq_cache[n - 1];
}
