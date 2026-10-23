/*
 * A118492  Number of Gaussian primes with floor(abs(p)) = n.
 * https://oeis.org/A118492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A077766(n) + 1 if n == 1 or n is prime ≡ 3 (mod 4);
 *            A077766(n) is the number of primes p ≡ 1 (mod 4) with n² ≤ p ≤
 *            (n+1)² − 1. Hence the sequence counts Gaussian primes whose
 *            floor(|p|) equals n
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   floor_sqrt_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/floor_sqrt_uint32.h"

/*==========================================================================*/
/*  Parameters for the precomputation                                       */
/*==========================================================================*/
static const uint64_t MAX_N = 10000ULL;   /* pre‑computed maximum index   */

/*==========================================================================*/
/*  Bit‑set helpers for a sieve                                            */
/*==========================================================================*/
static inline void set_bit(uint8_t *arr, uint32_t i) {
    arr[i >> 3] |= (1U << (i & 7));
}
static inline bool get_bit(const uint8_t *arr, uint32_t i) {
    return (arr[i >> 3] >> (i & 7)) & 1U;
}

/*==========================================================================*/
/*  Integer square root (no floating point)                               */

/*==========================================================================*/
/*  Sieve data                                                            */
/*==========================================================================*/
static uint8_t  *sieve_bits  = NULL;   /* bitset: 0=prime,1=composite   */
static uint32_t sweep_limit  = 0;      /* largest index in sieve_bits   */
static uint32_t *prime_mod1  = NULL;   /* primes ≡ 1 (mod 4)            */
static size_t   prime_mod1_cnt = 0;
static size_t   prime_mod1_cap = 0;

/*==========================================================================*/
/*  Initialise the sieve and collect the needed primes                     */
/*==========================================================================*/
static void init_sieve(void) {
    if (sieve_bits != NULL) return;            /* already initialised */

    /* limit for the sieve: (MAX_N+1)² − 1 */
    sweep_limit = (uint32_t)((MAX_N + 1U) * (MAX_N + 1U) - 1U);
    size_t bit_len = (sweep_limit >> 3) + 1;
    sieve_bits = (uint8_t*)malloc(bit_len);
    if (!sieve_bits) abort();
    memset(sieve_bits, 0, bit_len);           /* all 0 → candidate primes */

    /* mark 0 and 1 as composite */
    set_bit(sieve_bits, 0);
    set_bit(sieve_bits, 1);

    /* classic Eratosthenes */
    uint32_t sqrt_lim = floor_sqrt_uint32(sweep_limit);
    for (uint32_t i = 2; i <= sqrt_lim; ++i) {
        if (get_bit(sieve_bits, i)) continue;
        for (uint32_t j = i * i; j <= sweep_limit; j += i)
            set_bit(sieve_bits, j);
    }

    /* allocate array for primes ≡ 1 (mod 4) */
    prime_mod1_cap = 3000000U;                 /* initial capacity */
    prime_mod1 = (uint32_t*)malloc(prime_mod1_cap * sizeof(uint32_t));
    if (!prime_mod1) abort();

    /* collect the primes ≡ 1 (mod 4) */
    for (uint32_t i = 2; i <= sweep_limit; ++i) {
        if (get_bit(sieve_bits, i)) continue;  /* composite */
        if (i & 1U && (i % 4U) == 1U) {        /* odd and ≡1 (mod 4) */
            if (prime_mod1_cnt == prime_mod1_cap) {
                prime_mod1_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(prime_mod1,
                                                    prime_mod1_cap * sizeof(uint32_t));
                if (!tmp) abort();
                prime_mod1 = tmp;
            }
            prime_mod1[prime_mod1_cnt++] = i;
        }
    }
}

/*==========================================================================*/
/*  Binary search helpers on the prime list                                */
/*==========================================================================*/
static inline size_t lower_bound_uint32(const uint32_t *arr, size_t n, uint32_t val) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}
static inline size_t upper_bound_uint32(const uint32_t *arr, size_t n, uint32_t val) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] <= val) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/*==========================================================================*/
/*  Small primality test (used only up to 10 000)                            */
/*==========================================================================*/
static bool is_prime_small(uint32_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1U) == 0) return false;
    for (uint32_t i = 3; (uint64_t)i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

/*==========================================================================*/
/*  Main sequence function                                                  */
/*==========================================================================*/
seq_t a(idx_t n) {
    if (n == 0U) return 0;
    init_sieve();

    /* for indices beyond the pre‑computed range simply treat the special
       case; the tests are expected to stay within MAX_N.                     */
    if (n > MAX_N) {
        return (seq_t)((n == 1U) ||
                       ((n & 3U) == 3U && is_prime_small((uint32_t)n)));
    }

    uint32_t un = (uint32_t)n;                      /* safe: n ≤ MAX_N */
    uint32_t low  = un * un;
    uint32_t high = (un + 1U) * (un + 1U) - 1U;

    size_t l = lower_bound_uint32(prime_mod1, prime_mod1_cnt, low);
    size_t r = upper_bound_uint32(prime_mod1, prime_mod1_cnt, high);
    seq_t res = (seq_t)(r - l);

    if (un == 1U || ((un & 3U) == 3U && is_prime_small(un)))
        ++res;
    return res;
}
