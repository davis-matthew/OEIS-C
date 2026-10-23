/*
 * A132259  Isolated primes congruent to {1, 17, 19, 29} mod 30.
 * https://oeis.org/A132259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   realloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132259:
 *   a(n) = prime p such that
 *     p mod 30 ∈ {1,17,19,29}    (i.e. neither 5 nor 3 divides p)
 *     and neither p‑2 nor p+2 is prime.
 *   a(1) = 47, a(2) = 79, …
 * The function generates terms on demand, caching previously found
 * primes.  It works for any build where
 *   - idx_t is an unsigned integer type (e.g. uint64_t)
 *   - seq_t is an unsigned integer type or an arbitrary‑precision
 *     integer class (e.g. unsigned __int128 or a big integer class).
 * The code is valid C11 and C++17; all dynamic memory allocations
 * are cast appropriately for the C++ compiler.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/realloc_array.h"

/* static state ----------------------------------------------------- */
static idx_t *prime_list      = NULL;   /* primes up to the current sieve limit */
static size_t  prime_count    = 0;
static uint8_t *prime_flag    = NULL;   /* prime_flag[i] == 1 iff i is prime (array size = limit+3) */
static idx_t   prime_limit    = 0;     /* current sieve limit */
static size_t  prime_scan_idx = 0;     /* next index to examine in prime_list */

static seq_t *seq_cache      = NULL;   /* cached isolated primes */
static size_t  seq_len        = 0;     /* number of terms already cached */
static size_t  seq_cap        = 0;     /* allocated capacity of seq_cache */

static bool initialized = false;

/* ----------------------------------------------------------------- */
// Allocate/resize an array; used for both C and C++ (void* cast if needed)
static void* alloc_array(size_t nmemb, size_t size)
{
    return malloc(nmemb * size);
}

/* ----------------------------------------------------------------- */
// Build an Eratosthenes sieve up to LIMIT (inclusive).  prime_flag is
// allocated with size LIMIT+3 to allow checking p+2 safely.
static void build_prime_sieve(idx_t LIMIT)
{
    /* allocate the flag array */
    free(prime_flag);
    prime_flag = (uint8_t*)alloc_array((size_t)(LIMIT + 3), sizeof(uint8_t));
    if (!prime_flag) abort();
    memset(prime_flag, 1, (size_t)(LIMIT + 3));
    prime_flag[0] = prime_flag[1] = 0;

    /* sieve */
    for (idx_t i = 2; i * i <= LIMIT; ++i)
    {
        if (prime_flag[i])
        {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                prime_flag[j] = 0;
        }
    }

    /* build the prime list */
    free(prime_list);
    prime_count = 0;
    size_t cap = LIMIT / 10 + 16;
    prime_list = (idx_t*)alloc_array(cap, sizeof(idx_t));
    if (!prime_list) abort();

    for (idx_t i = 2; i <= LIMIT; ++i)
    {
        if (prime_flag[i])
        {
            if (prime_count == cap)
            {
                cap <<= 1;
                prime_list = (idx_t*)realloc_array(prime_list, cap * sizeof(idx_t));
                if (!prime_list) abort();
            }
            prime_list[prime_count++] = i;
        }
    }

    prime_limit = LIMIT;
    prime_scan_idx = 0;
}

/* ----------------------------------------------------------------- */
// Initialisation performed only on the first call.
static void init_once(void)
{
    if (initialized) return;

    /* 5 000 000 is comfortably large for the first 10 000 terms
       (the 10 000th isolated prime is below 300 000).          */
    build_prime_sieve(5000000);

    seq_cache = NULL;
    seq_len   = 0;
    seq_cap   = 0;

    initialized = true;
}

/* ----------------------------------------------------------------- */
// Append a new term to the cache, resizing if necessary.
static void append_term(seq_t val)
{
    if (seq_len == seq_cap)
    {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap << 1;
        seq_cache = (seq_t*)realloc_array(seq_cache, new_cap * sizeof(seq_t));
        if (!seq_cache) abort();
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = val;
}

/* ----------------------------------------------------------------- */
// Ensure that seq_cache contains at least n terms.
static void ensure_sequence(idx_t n)
{
    init_once();

    /* We may need to extend the sieve if we run out of primes. */
    for (;;)
    {
        /* Scan remaining primes */
        while (prime_scan_idx < prime_count && seq_len < n)
        {
            idx_t p = prime_list[prime_scan_idx++];
            /* residue condition */
            idx_t r = p % 30;
            if (r != 1 && r != 17 && r != 19 && r != 29)
                continue;

            /* Isolation check: p-2 and p+2 must not be prime */
            bool left_prime  = prime_flag[p - 2];
            bool right_prime = (p + 2 <= prime_limit) && prime_flag[p + 2];
            if (left_prime || right_prime)
                continue;

            append_term((seq_t)p);
        }

        if (seq_len >= n) break;   /* enough terms found */

        /* Not enough terms: increase sieve limit and rebuild all data.  */
        idx_t new_limit = prime_limit * 2;
        free(prime_flag);
        free(prime_list);
        prime_flag = NULL;
        prime_list = NULL;
        prime_limit = 0;

        build_prime_sieve(new_limit);

        /* Restart: clear old cache and start scanning from the beginning. */
        free(seq_cache);
        seq_cache = NULL;
        seq_len   = 0;
        seq_cap   = 0;
        prime_scan_idx = 0;
    }
}

/* ----------------------------------------------------------------- */
// Public sequence function
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    ensure_sequence(n);
    return seq_cache[n - 1];
}
