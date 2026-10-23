/*
 * A154545  Odd nonprimes k such that the smallest digit of k + the number of
 *          smallest digits of k is prime.
 * https://oeis.org/A154545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A154545: odd nonprime k such that
   (the smallest digit of k + the number of occurrences of that digit)
   is prime.  The function builds the sequence lazily and memoises
   the results for efficient repeated calls. */

#include <stdbool.h>
#include <stddef.h>

/* Provided utilities */
#include "../utils/is_prime.h"
#include "../utils/digit_frequencies.h"
#include "../utils/grow_seq.h"

/* Return the n-th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache for generated terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* current allocation size */
    static idx_t last_checked = 0; /* last number examined for inclusion */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        /* Find the next odd number after last_checked. */
        do {
            last_checked++;
        } while ((last_checked & 1) == 0);  /* skip evens */

        /* Skip prime numbers.  The test for 1 is handled correctly
           because is_prime(1) returns false. */
        if (is_prime(last_checked))
            continue;

        /* Examine the decimal digits of last_checked. */
        unsigned int freq[10] = {0};
        digit_frequencies((seq_t)last_checked, freq);

        /* Find the smallest digit that actually appears. */
        int d = 0;
        while (d < 10 && freq[d] == 0)
            d++;
        if (d == 10)          /* should not happen for a positive integer */
            continue;

        /* Compute the sum of that digit and its count. */
        int sum = d + (int)freq[d];

        /* Test if the sum is prime. */
        if (!is_prime((idx_t)sum))
            continue;         /* this number does not belong to the sequence */

        /* Append the number to the cache. */
        if (cache_len == cache_cap)
            grow_seq(&cache, &cache_cap, cache_len + 1);
        cache[cache_len++] = (seq_t)last_checked;
    }

    return cache[n - 1];
}
