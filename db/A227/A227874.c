/*
 * A227874  Numbers n such that tau(n+1) - tau(n) = -2, where tau(n) = the
 *          number of divisors of n (A000005).
 * https://oeis.org/A227874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A227874:
   a(n) = the nth integer m such that τ(m) - τ(m+1) = 2.
   The function searches forward, caches found terms, and returns the
   requested term on demand.  It works for seq_t being an arbitrary
   unsigned integer type (or arbitrary‑precision class) and idx_t being
   uint64_t.  All malloc/realloc calls are cast so the code compiles
   in both C11 and C++17. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Prime table (sieve of Eratosthenes) -------------------- */
static uint32_t *primes = NULL;
static size_t   primes_count = 0;

/* Build primes up to a fixed limit (1 000 000).  The limit is large
   enough to cover the square root of any n that fits in 64 bits. */
static void build_primes(void)
{
    if (primes != NULL)   /* already built */
        return;

    const uint32_t limit = 1000000U;

    /* Allocate boolean array for the sieve */
    bool *composite = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!composite) abort();

    for (uint32_t i = 2U; (uint64_t)i * i <= limit; ++i)
        if (!composite[i])
            for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                composite[(size_t)j] = true;

    /* Count primes to know the size of the primes array */
    for (uint32_t i = 2U; i <= limit; ++i)
        if (!composite[i])
            ++primes_count;

    primes = (uint32_t *)malloc(primes_count * sizeof(uint32_t));
    if (!primes) abort();

    /* Store the primes */
    size_t idx = 0;
    for (uint32_t i = 2U; i <= limit; ++i)
        if (!composite[i])
            primes[idx++] = i;

    free(composite);
}

/* ---------- Divisor function for 64‑bit numbers ------------------- */
static seq_t divisor_count_u64(uint64_t n)
{
    seq_t tau = 1U;
    uint64_t m = n;

    for (size_t i = 0U; i < primes_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint32_t p = primes[i];
        if (m % p == 0U) {
            unsigned exp = 0U;
            while (m % p == 0U) {
                m /= p;
                ++exp;
            }
            tau *= (seq_t)(exp + 1U);
        }
    }
    if (m > 1U)
        tau *= (seq_t)2U;

    return tau;
}

/* ---------- Main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0U)   /* sequence is defined for n >= 1 */
        return (seq_t)0U;

    /* Build the prime table on first use */
    if (primes == NULL)
        build_primes();

    /* Static cache of already discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0U;

    /* State of the incremental search */
    static uint64_t last_checked = 0U;   /* last integer examined      */
    static seq_t   tau_curr   = 0U;      /* τ(last_checked)            */
    static seq_t   tau_next   = 0U;      /* τ(last_checked + 1)        */

    /* Initialise the search on the first call */
    if (last_checked == 0U) {
        last_checked = 1U;
        tau_curr = divisor_count_u64(1U);
        tau_next = divisor_count_u64(2U);
    }

    /* Grow the cache until it holds at least n elements */
    while (cache_size < (size_t)n) {
        /* Record current integer if it satisfies the condition */
        if (tau_curr - tau_next == (seq_t)2U) {
            seq_t *tmp = (seq_t *)realloc(cache, (cache_size + 1U) * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache[cache_size] = (seq_t)last_checked;
            ++cache_size;
        }

        /* Advance to the next integer */
        ++last_checked;
        tau_curr = tau_next;
        tau_next = divisor_count_u64(last_checked + 1U);
    }

    return cache[n - 1U];
}
