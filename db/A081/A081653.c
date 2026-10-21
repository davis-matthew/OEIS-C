/*
 * A081653  Greatest common divisor of sums of decimal digits of n and of n-th
 *          prime.
 * https://oeis.org/A081653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = GCD( digit_sum(n), digit_sum( prime(n) ) )
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Forward declarations ---------------------------------------------------- */
static void init_primes(void);
static seq_t digit_sum_idx(idx_t v);
static seq_t seq_gcd_local(seq_t a, seq_t b);

/* Prime sieve ------------------------------------------------------------ */
#define PRIME_SIEVE_LIMIT 200000          /* 10000th prime = 104729 < 200000 */
static bool primes_ready = false;
static uint32_t primes_array[25000];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_ready) return;
    bool *sieve = (bool*)malloc(PRIME_SIEVE_LIMIT + 1);
    for (int i = 0; i <= PRIME_SIEVE_LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) sieve[j] = false;
        }
    }
    size_t idx = 0;
    for (int i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (sieve[i]) {
            primes_array[idx++] = (uint32_t)i;
        }
    }
    free(sieve);
    prime_cnt = idx;
    primes_ready = true;
}

/* Utility functions ------------------------------------------------------ */
static seq_t digit_sum_idx(idx_t v)
{
    seq_t sum = 0;
    while (v) {
        sum += (seq_t)(v % 10);
        v /= 10;
    }
    return sum;
}

static seq_t seq_gcd_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t sum_n = digit_sum_idx(n);

    init_primes();
    if ((size_t)n > prime_cnt) return 0;   /* safety, should not happen */

    uint32_t prime_val = primes_array[(size_t)n - 1];
    seq_t sum_p = digit_sum_idx((idx_t)prime_val);

    return seq_gcd_local(sum_n, sum_p);
}
