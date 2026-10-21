/*
 * A069794  Prime(n) and prime(n+2) use the same digits.
 * https://oeis.org/A069794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   freq_equal, grow_freq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the OEIS sequence A069794:
   it builds the list of primes p such that the prime two places ahead
   (i.e. the prime occurring after exactly two other primes) uses
   exactly the same multiset of decimal digits as p.  The function
   generates primes on demand and stores the qualifying primes
   in a dynamic array, returning the nth term (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/freq_equal.h"
#include "../utils/grow_freq_array.h"

/* ---------- local types and constants ---------- */
typedef uint64_t local_uint64;          /* used for internal calculations */

/* ---------- global dynamic arrays ---------- */
static local_uint64 *prime_vals   = NULL;    /* primes discovered so far          */
static size_t        prime_cnt    = 0;        /* number of stored primes           */
static size_t        prime_cap    = 0;        /* capacity of prime_vals array      */

static uint8_t *prime_freqs          = NULL; /* 10 bytes each: digit frequencies  */
static size_t  prime_freq_cap        = 0;     /* capacity of prime_freqs array     */

static local_uint64 *good_vals      = NULL;    /* qualifying primes for the sequence */
static size_t        good_cnt       = 0;        /* number of qualifying primes       */
static size_t        good_cap       = 0;        /* capacity of good_vals array       */

static local_uint64  next_candidate  = 2;      /* candidate for primality test      */

/* ---------- utility ---------------------------------------------------- */

/* Grow an array of uint64_t, returning the new pointer.  */
static local_uint64 *
grow_uint64_array(local_uint64 *arr, size_t *cap, size_t need)
{
    if (*cap >= need) return arr;
    size_t new_cap = (*cap) ? (*cap) << 1 : 1024;
    while (new_cap < need) new_cap <<= 1;
    local_uint64 *tmp = (local_uint64 *)realloc(arr, new_cap * sizeof(local_uint64));
    *cap = new_cap;
    return tmp;
}

/* Compute frequency table of decimal digits for a non‑negative integer. */
static void
freq_of(local_uint64 n, uint8_t *dest)
{
    for (int d = 0; d < 10; ++d) dest[d] = 0;
    if (n == 0) { dest[0] = 1; return; }
    while (n != 0) {
        dest[n % 10]++;
        n /= 10;
    }
}

/* Prime test by trial division using the primes already generated. */
static bool
is_prime(local_uint64 n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1ULL) == 0ULL) return false;
    local_uint64 r = (local_uint64)sqrt((long double)n);
    for (size_t i = 0; i < prime_cnt && prime_vals[i] <= r; ++i)
        if (n % prime_vals[i] == 0) return false;
    return true;
}

/* ---------- main routine --------------------------------------------- */

static void
ensure_terms(idx_t target)
{
    /* Ensure the list of qualifying primes contains at least 'target' elements. */
    while (good_cnt < (size_t)target) {

        /* Find next prime. */
        while (!is_prime(next_candidate))
            next_candidate += (next_candidate == 2ULL) ? 1ULL : 2ULL;

        /* Store the new prime and its digit frequency. */
        prime_vals = grow_uint64_array(prime_vals, &prime_cap, prime_cnt + 1);
        prime_freqs = grow_freq_array(prime_freqs, &prime_freq_cap, prime_cnt + 1);

        prime_vals[prime_cnt] = next_candidate;
        freq_of(next_candidate, &prime_freqs[prime_cnt * 10]);

        /* If we have at least 3 primes, compare the frequencies of
           prime(n) and prime(n+2).  */
        if (prime_cnt >= 2) {
            const uint8_t *freq1 = &prime_freqs[(prime_cnt - 2) * 10]; /* prime(n)   */
            const uint8_t *freq2 = &prime_freqs[prime_cnt * 10];       /* prime(n+2) */
            if (freq_equal(freq1, freq2)) {
                good_vals = grow_uint64_array(good_vals, &good_cap, good_cnt + 1);
                good_vals[good_cnt] = prime_vals[prime_cnt - 2];
                ++good_cnt;
            }
        }

        /* Prepare for the next candidate and advance the prime counter. */
        ++prime_cnt;
        next_candidate += (next_candidate == 2ULL) ? 1ULL : 2ULL;
    }
}

seq_t a(idx_t n)
{
    /* Lazy initialisation of dynamic arrays. */
    static bool first = true;
    if (first) {
        first = false;
        prime_vals   = (local_uint64 *)malloc(1024 * sizeof(local_uint64));
        prime_freqs  = (uint8_t *)malloc(1024 * 10 * sizeof(uint8_t));
        prime_cap    = 1024;
        prime_freq_cap = 1024;

        good_vals    = (local_uint64 *)malloc(256 * sizeof(local_uint64));
        good_cap     = 256;
        good_cnt     = 0;
    }

    /* Generate primes until we have enough qualifying terms. */
    ensure_terms(n);
    return (seq_t)good_vals[n - 1];
}
