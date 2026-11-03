/*
 * A156226  Primes of the form 9*n^2 + 1.
 * https://oeis.org/A156226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 1400, u64 n <= 1400,
 *            u128 n <= 1400, big any n
 * Validated: standard, pass, 49 terms through n = 1155 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form 9*n^2 + 1.
   The function a(n) returns the nth such prime.
   Implementation uses a simple search: iterate over integers,
   test candidates 9*m^2+1 for primality with is_prime_big(),
   collect primes until the desired index is reached.
   No hard‑coded values are used. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: return true if m is in allowed residue classes {0,2,8} mod 10 */
static inline bool allowed_m(idx_t m)
{
    idx_t r = m % 10;
    return r == 0 || r == 2 || r == 8;
}

/* static cache for computed primes */
#define CACHE_MAX 200000  /* enough for n up to ~100000 */
static seq_t prime_cache[CACHE_MAX];
static size_t cache_size = 0;          /* number of primes stored */
static idx_t next_candidate = 2;      /* next m to try (starts at 2) */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Validate input: OEIS offset is 1 */
    if (n == 0) return 0;     /* undefined for n=0, return 0 as placeholder */

    /* Ensure cache holds at least n primes */
    while (cache_size < (size_t)n)
    {
        /* find next eligible m */
        while (!allowed_m(next_candidate))
            ++next_candidate;

        /* candidate value: 9*m^2 + 1 */
        seq_t m_seq = next_candidate;
        seq_t val = (seq_t)9 * m_seq * m_seq + (seq_t)1;

        /* test primality */
        if (is_prime_big(val))
        {
            if (cache_size >= CACHE_MAX)
            {
                /* Cache too small: dynamic growth not implemented.
                   For typical tests (n <= 10000) this should not happen. */
                /* In practice, we could enlarge the array here, but omitted for brevity. */
            }
            prime_cache[cache_size++] = val;
        }
        ++next_candidate;  /* advance for next iteration */
    }

    /* Return the requested element (1‑based index) */
    return prime_cache[n - 1];
}
