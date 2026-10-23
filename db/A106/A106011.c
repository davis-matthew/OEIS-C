/*
 * A106011  Primes p such that 50*p - 3 and 50*p + 3 are both primes.
 * https://oeis.org/A106011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 385, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106011 – primes p such that 50*p-3 and 50*p+3 are both primes.
   Computed by a sequential search using a deterministic Miller‑Rabin test
   for 64‑bit unsigned integers.  The function caches values to support
   arbitrary calling order. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/*------------------------------------------------------------*/
/*   Deterministic Miller‑Rabin for 64‑bit primes              */
/*------------------------------------------------------------*/
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t pow_mod(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d)
    {
        if (d & 1) result = mul_mod(result, a, mod);
        a = mul_mod(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,
                                            17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    /* deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL,
                                     450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/*------------------------------------------------------------*/
/*   Cached search for the sequence                           */
/*------------------------------------------------------------*/
static seq_t *seq_buf   = NULL;
static size_t  seq_cap  = 0;
static size_t  seq_len  = 0;
static uint64_t next_candidate = 2;        /* next integer to test */

static void expand_table(idx_t need)
{
    while (seq_len < (size_t)need)
    {
        /* Find next prime p */
        while (!is_prime_uint64(next_candidate))
            ++next_candidate;

        uint64_t p = next_candidate;
        ++next_candidate;                 /* next search start */

        /* Check the two surrounding numbers */
        uint64_t a = 50ULL * p - 3ULL;
        uint64_t b = 50ULL * p + 3ULL;
        if (is_prime_uint64(a) && is_prime_uint64(b))
        {
            /* Grow the array if needed */
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *tmp = (seq_t*)realloc(seq_buf, seq_cap * sizeof(seq_t));
                if (!tmp) return;          /* allocation failure – give up */
                seq_buf = tmp;
            }
            seq_buf[seq_len++] = (seq_t)p;
        }
    }
}

/*------------------------------------------------------------*/
/*   Public function                                          */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out‑of‑range request */

    expand_table(n);
    return seq_buf[n - 1];               /* 1‑based indexing */
}
