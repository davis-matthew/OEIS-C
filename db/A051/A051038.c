/*
 * A051038  11-smooth numbers: numbers whose prime divisors are all <= 11.
 * https://oeis.org/A051038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 1035, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Generates 11‑smooth numbers (numbers all of whose prime
 * divisors are ≤ 11) in increasing order using the classic humble‑number
 * merge technique.  The sequence is memoised so that each value is
 * computed at most once, irrespective of the order of calls. */
#include <stdlib.h>
#include <string.h>

static const unsigned primes[5] = {2, 3, 5, 7, 11};

static idx_t   cache_len   = 0;   /* number of terms stored */
static idx_t   cache_cap   = 0;   /* allocated capacity */
static seq_t * cache_data  = NULL;/* array of 11‑smooth numbers */

static idx_t   idx_ptr[5]  = {0, 0, 0, 0, 0}; /* positions for each prime */
static seq_t   next_val[5] = {0, 0, 0, 0, 0}; /* next multiples */

/* Ensure the cache can hold at least 'need' terms.  The array is
 * grown to the next power of two larger than 'need'.  Existing terms
 * are copied to the new storage. */
static void
ensure_capacity(idx_t need)
{
    if (cache_cap >= (size_t)need) return;

    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < (size_t)need) new_cap <<= 1;

#if defined(__cplusplus)
    seq_t *new_data = new seq_t[new_cap];
    if (cache_data) {
        for (idx_t i = 0; i < cache_len; ++i) new_data[i] = cache_data[i];
        delete [] cache_data;
    }
    cache_data = new_data;
#else
    seq_t *new_data = (seq_t*)realloc(cache_data, new_cap * sizeof(seq_t));
    if (!new_data) exit(1);   /* allocation failure */
    cache_data = new_data;
#endif
    cache_cap = new_cap;
}

/* Initialise the first terms and auxiliary tables. */
static void
init_sequence(void)
{
    cache_data[0] = (seq_t)1;
    cache_len = 1;
    for (int i = 0; i < 5; ++i) {
        idx_ptr[i]  = 0;
        next_val[i] = (seq_t)primes[i];   /* 1 * prime */
    }
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* safety for out‑of‑range input */

    /* Make sure we have storage for 'n' terms. */
    if (cache_cap == 0) {
        /* First call: allocate a small array and initialise data. */
        ensure_capacity(n);
        init_sequence();
    } else if (cache_len < n) {
        /* Grow if necessary. */
        ensure_capacity(n);
    }

    /* Generate new terms until we have the n‑th one. */
    while (cache_len < n) {
        /* Find the smallest next value among all primes. */
        seq_t min_val = next_val[0];
        for (int i = 1; i < 5; ++i)
            if (next_val[i] < min_val) min_val = next_val[i];

        /* Store it as the next term of the sequence. */
        cache_data[cache_len] = min_val;

        /* Update all primes that produced this minimal value. */
        for (int i = 0; i < 5; ++i) {
            if (next_val[i] == min_val) {
                idx_ptr[i]++;
                next_val[i] = cache_data[idx_ptr[i]] * (seq_t)primes[i];
            }
        }
        cache_len++;
    }

    return cache_data[n - 1];
}
