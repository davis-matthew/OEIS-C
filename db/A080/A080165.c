/*
 * A080165  Primes having initial digits "10" in binary representation.
 * https://oeis.org/A080165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3330, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A080165 – primes whose binary representation starts with “10”.
   The implementation searches consecutive integers, checks the leading two
   bits and primality, and caches the results for fast repeated access. */

/* --- standard headers ------------------------------------------------------ */
#include <stdint.h>
#include <stdbool.h>

/* --- helper: floor of log2 for a 64‑bit integer ----------------------------- */
static unsigned floor_log2_uint64(uint64_t x)
{
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

/* --- primality test for 64‑bit numbers ------------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* --- check whether the most significant two bits of x are “10” ------------ */
static bool has_leading10(uint64_t x)
{
    if (x < 2) return false;          /* 1 has only one bit */
    unsigned high = floor_log2_uint64(x);          /* position of the highest set bit */
    uint64_t pow_high = ((uint64_t)1) << high;    /* 2^(L-1)  */
    uint64_t pow_2   = ((uint64_t)1) << (high - 1);/* 2^(L-2)  */
    return x < pow_high + pow_2;            /* true iff binary starts with 10 */
}

/* --- static cache of found primes ------------------------------------------ */
#define MAX_CACHE 200000              /* sufficient for n≈10000       */
static seq_t prime_cache[MAX_CACHE];
static idx_t cached = 0;              /* number of primes already found */
static uint64_t next_candidate = 2;   /* integer to test next           */

/* --- required sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* by convention; sequence is 1‑based */

    while (cached < n)
    {
        /* advance to the next integer whose binary prefix is "10"           */
        while (!has_leading10(next_candidate)) ++next_candidate;

        /* test primality and store if prime                               */
        if (is_prime_uint64(next_candidate))
        {
            if (cached == MAX_CACHE) /* safety check */
                return 0;            /* or handle overflow appropriately */
            prime_cache[cached] = (seq_t)next_candidate;
            ++cached;
        }
        ++next_candidate;
    }
    return prime_cache[n - 1];
}
