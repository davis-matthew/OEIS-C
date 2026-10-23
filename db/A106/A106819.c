/*
 * A106819  Largest prime of the set of three consecutive primes whose sum of
 *          digits is a set of three distinct primes.
 * https://oeis.org/A106819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by searching consecutive triples of primes whose digit
 * sums are all distinct primes.  For each such triple (p1,p2,p3) the
 * function returns the third prime p3.  The sequence is built once
 * and cached. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Upper bound for the sieve; sufficient for the first 10 000
 * terms (primes below 30 000 000 give more than 1.8 million primes). */
#define SIEVE_LIMIT 30000000U

/* Prime list and their digit sums */
static uint32_t *prime_list   = NULL;
static size_t   prime_cap    = 0;
static size_t   prime_cnt    = 0;
static uint8_t *prime_dsum   = NULL;
static size_t   dsum_cap     = 0;
static size_t   dsum_cnt     = 0;

/* Result sequence stored as 32‑bit values; casted to seq_t on return */
static uint32_t *result_list32  = NULL;
static size_t   result_cap32    = 0;
static size_t   result_cnt32    = 0;

/* Small primality table for digit sums 0..80 */
static bool digit_sum_is_prime[81] = {0};

/* ------------------------------------------------------------------ */
/* Dynamically grow the prime list                                     */
static void push_prime(uint32_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
        prime_list = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
    }
    prime_list[prime_cnt++] = p;
}

static void push_dsum(uint8_t d)
{
    if (dsum_cnt == dsum_cap) {
        dsum_cap = (dsum_cap == 0) ? 16 : dsum_cap * 2;
        prime_dsum = (uint8_t *)realloc(prime_dsum, dsum_cap * sizeof(uint8_t));
    }
    prime_dsum[dsum_cnt++] = d;
}

/* Grow the result list (32‑bit) */
static void push_result32(uint32_t p)
{
    if (result_cnt32 == result_cap32) {
        result_cap32 = (result_cap32 == 0) ? 16 : result_cap32 * 2;
        result_list32 = (uint32_t *)realloc(result_list32, result_cap32 * sizeof(uint32_t));
    }
    result_list32[result_cnt32++] = p;
}

/* ------------------------------------------------------------------ */
/* Helper: digit sum of a 32‑bit integer                              */
static inline uint8_t digit_sum(uint32_t n)
{
    uint8_t s = 0;
    while (n) {
        s += (uint8_t)(n % 10);
        n /= 10;
    }
    return s;
}

/* ------------------------------------------------------------------ */
/* One‑time initialization                                            */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized) return;

    /* 1. Pre‑compute small primes up to 80 for digit‑sum checks */
    for (int i = 2; i < 81; ++i) {
        bool pr = true;
        for (int j = 2; j * j <= i; ++j)
            if (i % j == 0) { pr = false; break; }
        digit_sum_is_prime[i] = pr;
    }

    /* 2. Sieve of Eratosthenes up to SIEVE_LIMIT */
    uint8_t *composite = (uint8_t *)calloc(SIEVE_LIMIT + 1, 1);
    if (!composite) return;   /* allocation failure – unlikely */

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                composite[j] = 1;
        }
    }

    /* 3. Store primes and their digit sums */
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!composite[i]) {
            uint32_t p = (uint32_t)i;
            push_prime(p);
            push_dsum(digit_sum(p));
        }
    }
    free(composite);

    /* 4. Generate the sequence by examining consecutive triples */
    for (size_t i = 0; i + 2 < prime_cnt; ++i) {
        uint8_t s0 = prime_dsum[i];
        uint8_t s1 = prime_dsum[i + 1];
        uint8_t s2 = prime_dsum[i + 2];
        if (s0 != s1 && s0 != s2 && s1 != s2 &&
            digit_sum_is_prime[s0] &&
            digit_sum_is_prime[s1] &&
            digit_sum_is_prime[s2]) {
            push_result32(prime_list[i + 2]);   /* largest prime of triple */
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Public function: a(idx_t n)                                       */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* below offset – undefined */

    init_sequence();

    if ((idx_t)n > result_cnt32) return 0; /* term beyond pre‑computed range */

    return (seq_t)result_list32[(size_t)n - 1];
}
