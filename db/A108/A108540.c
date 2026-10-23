/*
 * A108540  Golden semiprimes: a(n)=p*q and abs(p*phi-q)<1, where phi = golden
 *          ratio = (1+sqrt(5))/2.
 * https://oeis.org/A108540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p*q where p and q are primes and |p*φ – q| < 1, φ =
 *            (1+√5)/2. The terms are produced in increasing order by
 *            examining the primes p in ascending order and testing both the
 *            floor and the ceil of p*φ
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define SIEVE_LIMIT 8000000ULL       /* Large enough for n ≤ 10000 */

/* Global state */
static bool initialized = false;
static uint8_t *is_prime_arr = NULL;      /* 1 = prime, 0 = composite */
static uint32_t *prime_list = NULL;        /* List of primes up to SIEVE_LIMIT */
static size_t prime_cnt = 0;               /* Number of primes in prime_list */
static size_t prime_idx = 0;               /* Next prime to process */

static seq_t *golden = NULL;              /* Array of found terms */
static size_t golden_size = 0;             /* Number of terms stored */
static size_t golden_cap  = 0;             /* Current capacity of golden[] */
static const long double PHI = (1.0L + sqrtl(5.0L)) / 2.0L;

/* Simple sieve of Eratosthenes up to SIEVE_LIMIT.  is_prime_arr and
   prime_list are filled. */
static void build_sieve(void)
{
    is_prime_arr = (uint8_t*)calloc(SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (!is_prime_arr) exit(1);

    /* Mark all odd numbers initially as prime */
    for (uint64_t i = 3; i <= SIEVE_LIMIT; i += 2) is_prime_arr[i] = 1;
    is_prime_arr[2] = 1;

    for (uint64_t i = 3; i * i <= SIEVE_LIMIT; i += 2) {
        if (is_prime_arr[i]) {
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i << 1)
                is_prime_arr[j] = 0;
        }
    }

    /* Count primes */
    prime_cnt = 1;  /* counting prime 2 */
    for (uint64_t i = 3; i <= SIEVE_LIMIT; i += 2)
        if (is_prime_arr[i]) ++prime_cnt;

    /* Allocate prime list */
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) exit(1);

    prime_list[0] = 2;
    size_t idx = 1;
    for (uint64_t i = 3; i <= SIEVE_LIMIT; i += 2)
        if (is_prime_arr[i]) prime_list[idx++] = (uint32_t)i;
}

/* Ensure that golden[0..n-1] contains at least n terms. */
static void ensure_golden(idx_t n)
{
    if (!initialized) {
        build_sieve();
        golden_cap = 128;
        golden = (seq_t*)malloc(golden_cap * sizeof(seq_t));
        if (!golden) exit(1);
        initialized = true;
    }

    while (golden_size < (size_t)n) {
        if (prime_idx >= prime_cnt) {
            /* No more primes in the table – should not occur for
               n ≤ 10000 with the chosen SIEVE_LIMIT. */
            exit(1);
        }
        uint32_t p = prime_list[prime_idx++];
        long double x = (long double)p * PHI;

        uint64_t q_floor = (uint64_t)floorl(x);
        uint64_t q_ceil  = q_floor + 1;

        /* Test floor candidate */
        if (q_floor <= SIEVE_LIMIT && is_prime_arr[q_floor]) {
            seq_t val = (seq_t)p * (seq_t)q_floor;
            if (golden_size >= golden_cap) {
                golden_cap *= 2;
                golden = (seq_t*)realloc(golden,
                                         golden_cap * sizeof(seq_t));
                if (!golden) exit(1);
            }
            golden[golden_size++] = val;
            if (golden_size >= (size_t)n) break;
        }

        /* Test ceil candidate (only if different) */
        if (q_ceil != q_floor &&
            q_ceil <= SIEVE_LIMIT && is_prime_arr[q_ceil]) {
            seq_t val = (seq_t)p * (seq_t)q_ceil;
            if (golden_size >= golden_cap) {
                golden_cap *= 2;
                golden = (seq_t*)realloc(golden,
                                         golden_cap * sizeof(seq_t));
                if (!golden) exit(1);
            }
            golden[golden_size++] = val;
            if (golden_size >= (size_t)n) break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for non‑offset access */
    ensure_golden(n);
    return golden[n - 1];
}
