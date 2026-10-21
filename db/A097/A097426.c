/*
 * A097426  Integer part of the circumference of circles with prime diameters.
 * https://oeis.org/A097426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n) * Pi)
 * Range:     u8 n <= 22, u16 n <= 2346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation generates primes on demand by simple trial division
 * and multiplies using a fixed‑point representation of Pi (scaled by 10^15)
 * to obtain the exact floor value in seq_t.  It works for any seq_t width
 * supported by the build (uint8_t …, unsigned __int128, arbitrary precision). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------
 * Prime generation
 * ------------------------------------------------------------------ */

static uint64_t *prime_arr  = (uint64_t *)0;   /* array of discovered primes */
static size_t   prime_cnt  = 0;                /* number of primes stored    */
static size_t   prime_cap  = 0;                /* allocated capacity         */
static uint64_t next_candidate = 2;            /* next integer to test       */

/* Ensure that at least `needed` primes are available in prime_arr. */
static void ensure_primes(size_t needed)
{
    /* Initialise storage if required. */
    if (prime_arr == (uint64_t *)0) {
        prime_cap = 128;
        prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    }

    /* Generate primes until the desired count is reached. */
    while (prime_cnt < needed) {
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > next_candidate) break;
            if (next_candidate % p == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1; /* double capacity */
                prime_arr = (uint64_t *)realloc(prime_arr,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_arr[prime_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------
 * Sequence a(n)
 * ------------------------------------------------------------------ */

/* Fixed‑point representation of π:  π ≈ PI_NUM / SCALE  (scale = 10^15) */
static const unsigned long long PI_NUM  = 3141592653589793ULL; /* π×10^15   */
static const unsigned long long SCALE  = 1000000000000000ULL; /* 10^15     */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* defined for safety, offset is 1 */

    /* Generate the required prime. */
    ensure_primes((size_t)n);
    uint64_t p = prime_arr[n - 1];      /* primes are 1‑based in OEIS */

    /* Compute floor(p * π) accurately using fixed‑point arithmetic. */
    unsigned __int128 prod = (unsigned __int128)p * PI_NUM;
    return (seq_t)(prod / SCALE);
}
