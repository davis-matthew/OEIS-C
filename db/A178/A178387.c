/*
 * A178387  Primes p such that q*p+-Mod(p,q) are primes, for q=7.
 * https://oeis.org/A178387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 7·p ± (p mod 7) are both prime (q = 7).
   Implementation sieve primes up to a fixed limit, test candidates,
   cache qualifying primes.  The function a(n) returns the nth term. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* ------------------------------------------------------------------ */
/* Sieve and prime list                                              */
/* ------------------------------------------------------------------ */
static uint8_t *sieve = NULL;          /* bitset of odd composites */
static size_t   sieve_bytes = 0;
static idx_t    sieve_limit = 0;       /* current sieve limit          */

static uint32_t *primes = NULL;        /* list of primes            */
static size_t   primes_count = 0;      /* number of primes found     */
static size_t   primes_cap = 0;        /* allocated capacity         */

/* ------------------------------------------------------------------ */
/* Sequence cache                                                    */
/* ------------------------------------------------------------------ */
static uint64_t *seq_cache64 = NULL;   /* cached qualifying primes   */
static size_t   seq_len = 0;           /* number of cached terms     */
static size_t   seq_cap = 0;           /* allocated capacity         */

/* ------------------------------------------------------------------ */
/* Index of next prime to examine in the primes array                */
/* ------------------------------------------------------------------ */
static size_t prime_idx = 0;

/* ------------------------------------------------------------------ */
/* Initialise sieve and primes list                                   */
/* ------------------------------------------------------------------ */
static void init_sieve_and_primes(void)
{
    if (sieve_limit != 0)
        return;                        /* already initialised */

    /* Choose a limit that comfortably yields >10 000 qualifying primes.
       20 000 000 gives roughly 1.3 M primes, well more than needed.      */
    sieve_limit = 20000000;

    /* Number of odd integers ≥3 and ≤sieve_limit */
    size_t odd_count = 0;
    if (sieve_limit >= 3)
        odd_count = ((sieve_limit - 3) >> 1) + 1;

    sieve_bytes = (odd_count + 7) >> 3;
    sieve = (uint8_t*)malloc(sieve_bytes);
    for (size_t i = 0; i < sieve_bytes; ++i)
        sieve[i] = 0;

    /* Bit access helpers */
    #define SIEVE_SET(idx)   sieve[(idx) >> 3] |= (1 << ((idx) & 7))
    #define SIEVE_GET(idx)   ((sieve[(idx) >> 3] >> ((idx) & 7)) & 1)

    /* Eratosthenes sieve over odd numbers only */
    for (size_t i = 3; i * i <= (size_t)sieve_limit; i += 2) {
        size_t idx_i = (i - 3) >> 1;
        if (!SIEVE_GET(idx_i)) {
            for (size_t j = i * i; j <= (size_t)sieve_limit; j += i << 1) {
                size_t idx_j = (j - 3) >> 1;
                SIEVE_SET(idx_j);
            }
        }
    }

    /* Collect primes into array */
    primes_cap = 1400000;                  /* initial estimate */
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    primes[0] = 2;
    primes_count = 1;

    for (size_t idx = 0; idx < odd_count; ++idx) {
        if (!SIEVE_GET(idx)) {
            uint32_t p = (uint32_t)((idx << 1) + 3);
            if (primes_count == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_count++] = p;
        }
    }

    #undef SIEVE_SET
    #undef SIEVE_GET
}

/* ------------------------------------------------------------------ */
/* Sequence function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_sieve_and_primes();

    /* Extend cache until it holds at least n terms */
    while (seq_len < n) {
        if (prime_idx >= primes_count)
            return 0;                       /* not enough data */

        uint32_t p = primes[prime_idx++];
        uint32_t r = p % 7;

        uint64_t val1 = 7ull * p + r;
        uint64_t val2 = 7ull * p - r;

        if (is_prime_big((seq_t)val1) && is_prime_big((seq_t)val2)) {
            if (seq_len >= seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 8;
                seq_cache64 = (uint64_t*)realloc(seq_cache64,
                                                seq_cap * sizeof(uint64_t));
            }
            seq_cache64[seq_len++] = p;
        }
    }

    return (seq_t)seq_cache64[n - 1];
}
