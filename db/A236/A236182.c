/*
 * A236182  Sum of the sixth powers of the first n primes.
 * https://oeis.org/A236182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} prime(k)^6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 165, u128 n <= 1000,
 *            big any n
 * Helpers:   grow_uint32_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the sum of the sixth powers of the first n primes.
   It memoises primes and prefix sums across calls, so repeated calls with
   arbitrary indices are fast.  The implementation uses only the operations
   allowed for seq_t and idx_t, and works for any unsigned integer type
   (including arbitrary‑precision). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_uint32_arr.h"

/* Grow helpers – expand an array to at least min_needed capacity. */
static void grow_seq_arr(seq_t **arr, size_t *cap, size_t min_needed)
{
    if (*cap >= min_needed) return;
    size_t new_cap = (*cap > 0) ? *cap * 2 : 16;
    if (new_cap < min_needed) new_cap = min_needed;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (tmp) { *arr = tmp; *cap = new_cap; }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence starts at offset 1 */

    /*  static memoised data */
    static seq_t *prefix = NULL;   /* prefix sums of prime^6              */
    static size_t prefix_cap = 0;
    static uint32_t *primes = NULL;/* list of primes discovered so far    */
    static size_t primes_cap = 0;
    static size_t primes_len = 0;  /* number of primes currently stored   */
    static uint32_t next_candidate = 0;   /* candidate for the next prime */

    /* Ensure storage is large enough for the requested index */
    grow_seq_arr(&prefix, &prefix_cap, (size_t)n);
    grow_uint32_arr(&primes, &primes_cap, (size_t)n);

    /* Generate missing primes and their prefix sums */
    while (primes_len < (size_t)n)
    {
        uint32_t candidate;

        if (primes_len == 0)
            candidate = 2;
        else
            candidate = next_candidate;   /* next odd candidate */

        /* primality test – simple trial division by known primes */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }

        if (is_prime)                  /* store prime and update prefix */
        {
            primes[primes_len] = candidate;
            seq_t p = (seq_t)candidate;
            seq_t p6 = p * p * p * p * p * p;   /* compute prime^6 */
            if (primes_len == 0)
                prefix[0] = p6;
            else
                prefix[primes_len] = prefix[primes_len-1] + p6;
            ++primes_len;

            /* next candidate: 3 after 2, otherwise candidate+2 */
            if (candidate == 2) next_candidate = 3;
            else                next_candidate = candidate + 2;
        }
        else                            /* candidate not prime – skip */
        {
            if (candidate == 2) next_candidate = 3;
            else                next_candidate = candidate + 2;
        }
    }

    return prefix[n-1];   /* a(n) = sum of first n prime⁶ terms */
}
