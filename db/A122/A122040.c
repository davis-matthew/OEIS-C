/*
 * A122040  Numbers which are equal to the arithmetic mean of six successive
 *          primes.
 * https://oeis.org/A122040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A122040: Numbers which are equal to the arithmetic mean of six
 * successive primes.
 *
 * Idea: Iterate over the primes, keeping a sliding window of six primes.
 * Whenever the sum of the six primes is divisible by 6, the integer mean
 * is recorded.  The resulting values are stored in a cache so that
 * a future call for any n simply returns the cached value or continues
 * generating more terms if needed.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Cached sequence values */
static seq_t *seq_cache      = NULL;
static size_t seq_cache_len  = 0;
static size_t seq_cache_cap  = 0;

/* Prime list */
static uint64_t *prime_list  = NULL;
static size_t   prime_len     = 0;
static size_t   prime_cap     = 0;

/* Sliding window state */
static uint64_t window_start = 0;          /* index of first prime in the window */
static uint64_t sliding_sum  = 0;          /* current sum of the six primes */

/* Ensure the prime array has at least 'needed' slots. */
static void ensure_prime_cap(size_t needed)
{
    if (prime_cap >= needed) return;
    size_t new_cap = prime_cap ? prime_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) return;                        /* allocation failure – ignore */
    prime_list = tmp;
    prime_cap  = new_cap;
}

/* Ensure the sequence cache has at least 'needed' slots. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cache_cap >= needed) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                        /* allocation failure – ignore */
    seq_cache   = tmp;
    seq_cache_cap = new_cap;
}

/* Append a new prime to the prime list. */
static void append_prime(uint64_t p)
{
    ensure_prime_cap(prime_len + 1);
    prime_list[prime_len++] = p;
}

/* Append a new sequence value to the cache. */
static void append_seq_val(seq_t v)
{
    ensure_seq_cap(seq_cache_len + 1);
    seq_cache[seq_cache_len++] = v;
}

/* Generate primes until at least 'needed' primes are in prime_list. */
static void generate_primes(size_t needed)
{
    if (prime_len >= needed) return;
    uint64_t last = (prime_len > 0) ? prime_list[prime_len - 1] : 1;
    while (prime_len < needed) {
        last = next_prime(last);
        append_prime(last);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* undefined for n<1 */

    /* Return cached value if already computed. */
    if (seq_cache_len >= (size_t)n)
        return seq_cache[n - 1];

    /* Ensure we have the first six primes to start the window. */
    generate_primes(6);

    /* Initialise window sum the first time we align the window. */
    if (window_start == 0 && sliding_sum == 0) {
        for (size_t i = 0; i < 6; ++i)
            sliding_sum += prime_list[i];
        if (sliding_sum % 6 == 0)
            append_seq_val((seq_t)(sliding_sum / 6));
    }

    /* Emit terms until the cache contains at least n values. */
    while (seq_cache_len < (size_t)n) {
        /* Generate the next prime to slide the window. */
        uint64_t new_prime = next_prime(prime_list[prime_len - 1]);
        append_prime(new_prime);

        /* Update sliding window sum. */
        sliding_sum += new_prime;
        sliding_sum -= prime_list[window_start];
        ++window_start;

        /* Record the mean if it is an integer. */
        if (sliding_sum % 6 == 0)
            append_seq_val((seq_t)(sliding_sum / 6));
    }

    return seq_cache[n - 1];
}
