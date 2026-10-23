/*
 * A115176  Positive numbers that are not the sum of two squares and a
 *          positive Fibonacci number.
 * https://oeis.org/A115176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A115176:
   a(n) = the n‑th positive integer that cannot be written as
          x^2 + y^2 + F where x,y are non‑negative integers and F is a
          positive Fibonacci number.  The implementation iterates
          successive candidates and tests all Fibonacci summands; a
          candidate is representable iff one of cand‑F is a sum of two
          squares.  Sums of two squares are detected by prime
          factorisation: a number is representable iff every prime
          ≡ 3 (mod 4) occurs with an even exponent.  The routine
          memoises already found terms so that calls with arbitrary
          orders are efficient.  No external libraries are required
          beyond the standard C/C++ headers. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* --- Prime table (up to a small fixed limit) -------------------------------- */

#define MAX_PRIME_LIMIT 10000          /* sqrt of the max candidate we allow */
static uint32_t primes[MAX_PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    /* Simple trial‑division sieve, cost negligible for the small limit */
    for (uint32_t p = 2; p <= MAX_PRIME_LIMIT; ++p)
    {
        bool composite = false;
        uint32_t r = (uint32_t)sqrt((double)p);
        for (size_t i = 0; i < prime_cnt && primes[i] <= r; ++i)
        {
            if (p % primes[i] == 0)
            {
                composite = true;
                break;
            }
        }
        if (!composite)
            primes[prime_cnt++] = p;
    }
}

/* --- Fibonacci numbers (positive ones) -------------------------------------- */

static uint64_t fibs[64];
static size_t fib_cnt = 0;

static void init_fibs(uint64_t limit)
{
    fibs[0] = 1;
    fibs[1] = 1;
    fib_cnt = 2;
    while (1)
    {
        uint64_t next = fibs[fib_cnt - 1] + fibs[fib_cnt - 2];
        if (next > limit) break;
        fibs[fib_cnt++] = next;
    }
}

/* --- Check if m is a sum of two squares ----------------------------------- */

static bool is_sum_of_two_squares(uint64_t m)
{
    if (m == 0) return true;          /* 0 = 0^2 + 0^2 */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > m) break;
        if (m % p == 0)
        {
            int exp = 0;
            do { m /= p;  ++exp; }
            while (m % p == 0);
            if ((p % 4 == 3) && (exp & 1))
                return false;           /* odd exponent of p ≡ 3 (mod 4) */
        }
    }
    /* Remaining factor (if >1) is prime */
    if (m > 1 && (m % 4 == 3))
        return false;
    return true;
}

/* --- Cached sequence storage ---------------------------------------------- */

static seq_t *cache = NULL;          /* dynamic array of found terms */
static size_t cache_len = 0;         /* number of terms stored   */
static size_t cache_cap = 0;         /* allocated capacity       */
static uint64_t next_candidate = 1;  /* next integer to test      */

/* --- Helper to grow the cache --------------------------------------------- */

static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 1024 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* Out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* --- Main sequence routine ------------------------------------------------- */

seq_t a(idx_t n)
{
    /* On first call initialise shared data structures once */
    static bool initialized = false;
    static uint64_t max_limit = 2000000;     /* large enough for ~10 000 terms */

    if (!initialized)
    {
        init_primes();                          /* primes up to 10 000 */
        init_fibs(max_limit);                   /* Fibonacci numbers ≤ limit */
        cache_cap = 0;
        cache = NULL;
        initialized = true;
    }

    /* Return from cache if already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Test whether next_candidate is representable */
        bool representable = false;
        for (size_t i = 0; i < fib_cnt && fibs[i] <= next_candidate; ++i)
        {
            uint64_t r = next_candidate - fibs[i];
            if (is_sum_of_two_squares(r))
            {
                representable = true;
                break;
            }
        }

        if (!representable)
        {
            /* Store in cache, growing if necessary */
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* Advance to next integer */
        ++next_candidate;
        /* If we exceeded the pre‑generated Fibonacci bound, extend it */
        if (next_candidate > max_limit)
        {
            /* Double the limit to keep growth manageable */
            max_limit *= 2;
            init_fibs(max_limit);
        }
    }

    /* Now cache_len >= n */
    return cache[n - 1];
}
