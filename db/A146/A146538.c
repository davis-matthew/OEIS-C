/*
 * A146538  Even numbers n such that n+3 is not a prime.
 * https://oeis.org/A146538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th even integer such that (n + 3) is not prime
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple trial‑division primality test for
   the numbers involved; all terms needed for n ≤ 10 000 are less than
   three digits, so the test is fast enough.  No external helper
   functions are required beyond the standard C headers.  The result
   is cached so that repeated calls for arbitrary indices are
   inexpensive. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Simple primality test for 64‑bit integers. */
static bool isPrimeSmall(uint64_t n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if ((n & 1ULL) == 0) return false;
    for (uint64_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* cache of previously computed terms; indices start at 1 */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t count = 0;
    static seq_t next_even = (seq_t)6;   /* first even to test */

    if (n == 0) return 0;   /* defensive: the sequence is 1‑based */

    /* Grow cache array as needed */
    if (capacity < n)
    {
        size_t new_cap = capacity ? capacity * 2 : 64;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
    }

    /* Generate terms up to the requested index */
    while (count < n)
    {
        /* Skip even numbers whose (even + 3) is a prime */
        while (isPrimeSmall((uint64_t)(next_even + (seq_t)3)))
            next_even += (seq_t)2;
        cache[count] = next_even;
        ++count;
        next_even += (seq_t)2;
    }

    return cache[n - 1];
}
