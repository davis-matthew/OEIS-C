/*
 * A077718  Primes which can be expressed as sum of distinct powers of 4.
 * https://oeis.org/A077718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime whose base‑4 representation consists only
 *            of digits 0 and 1
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 3348, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul, value_from_bin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates numbers of the form Σ 4^k (with each k used at most once) in
   increasing order by iterating over binary representations, tests each for primality
   using a deterministic Miller–Rabin implementation for 64‑bit integers, and caches results
   for lazy evaluation.  The cache supports arbitrary ordering of calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mod_mul.h"
#include "../utils/value_from_bin.h"

/* cache for sequence values */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;   /* number of values cached */
static size_t seq_cap     = 0;   /* allocated capacity */
static uint64_t next_bin  = 1;   /* next binary index to process */

/* modular multiplication using 128‑bit intermediate */

/* modular exponentiation */
static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1)
            r = mod_mul(r, a, mod);
        a = mod_mul(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* deterministic Miller–Rabin for 64‑bit integers (valid up to 3.5×10¹²). */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0ULL)
            return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0ULL)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp)
            return false;
    }
    return true;
}

/* wrapper for seq_t values (assumes value fits in 64 bits). */
static bool is_prime_seq(seq_t v)
{
    return is_prime_u64((uint64_t)v);
}

/* enlarge the cache if needed */
static void grow_cache(void)
{
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)
        abort();            /* allocation failure – should not happen in tests */
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* generate terms until the cache contains at least n values */
static void ensure_terms(idx_t n)
{
    while (seq_len < n)
    {
        seq_t val = value_from_bin(next_bin);
        if (is_prime_seq(val))
        {
            if (seq_len == seq_cap)
                grow_cache();
            seq_cache[seq_len++] = val;
        }
        ++next_bin;
    }
}

/* Main sequence function requested by the framework */
seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for indices below the offset */
        return 0;
    ensure_terms(n);
    return seq_cache[n - 1];
}
