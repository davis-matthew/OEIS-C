/*
 * A109559  prime(k) for those k where
 *          floor((2*(prime(k+1)-prime(k))*PrimePi(k) mod (8*k))/k) = m with m
 *          = 5.
 * https://oeis.org/A109559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Seq: A109559 – primes p(k) for which
 * floor((2*(p(k+1)-p(k))*π(k) mod (8*k))/k) = 5.
 * The sequence a(n) lists these primes in order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const int SIEVE_BOUND = 5000000;          /* safe upper limit */
static uint8_t *is_prime   = NULL;
static uint32_t *pi_arr    = NULL;
static uint64_t *prime_arr = NULL;
static uint32_t prime_cnt = 0;

static seq_t  *qual_primes = NULL;
static idx_t   qual_cap    = 0;
static idx_t   qual_size   = 0;
static idx_t   last_k      = 0;
static bool    initialized = false;

static void
init_sequence(void)
{
    int i, j;
    is_prime = (uint8_t *)malloc(SIEVE_BOUND + 1);
    pi_arr    = (uint32_t *)malloc((SIEVE_BOUND + 1) * sizeof(uint32_t));
    prime_arr = (uint64_t *)malloc(SIEVE_BOUND * sizeof(uint64_t)); /* rough size */

    /* Sieve of Eratosthenes */
    for (i = 0; i <= SIEVE_BOUND; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; (long long)i * i <= SIEVE_BOUND; ++i)
        if (is_prime[i])
            for (j = i * i; j <= SIEVE_BOUND; j += i)
                is_prime[j] = 0;

    /* Collect primes into array and build π array */
    prime_cnt = 0;
    pi_arr[0] = 0;
    for (i = 1; i <= SIEVE_BOUND; ++i) {
        pi_arr[i] = pi_arr[i - 1] + is_prime[i];
        if (is_prime[i]) {
            prime_arr[prime_cnt++] = (uint64_t)i;
        }
    }

    /* Prepare qualification array */
    qual_cap = 1024;
    qual_primes = (seq_t *)malloc(qual_cap * sizeof(seq_t));
    qual_size   = 0;
    last_k      = 0;
    initialized = true;
}

static void
push_qual(seq_t v)
{
    if (qual_size == qual_cap) {
        qual_cap <<= 1;
        qual_primes = (seq_t *)realloc(qual_primes, qual_cap * sizeof(seq_t));
    }
    qual_primes[qual_size++] = v;
}

static void
ensure_qual_needed(idx_t need)
{
    while (qual_size < need) {
        ++last_k;                       /* k starts from 1 */
        if (last_k >= prime_cnt - 1) {   /* need next prime to compute gap */
            /* In practice the defined bound suffices */
            break;
        }
        uint64_t pk   = prime_arr[last_k - 1];
        uint64_t pk1  = prime_arr[last_k];     /* p(k+1) */
        uint64_t gap  = pk1 - pk;
        uint32_t pi_k = pi_arr[last_k];        /* π(k) */

        __uint128_t prod  = (__uint128_t)2 * gap * pi_k;
        uint64_t mod      = (uint64_t)(prod % ((uint64_t)8 * last_k));
        uint64_t div_floor = mod / last_k;

        if (div_floor == 5) {
            push_qual((seq_t)pk);
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0) return 0;                      /* out of range, harmless */
    ensure_qual_needed(n);
    return qual_primes[n - 1];
}
