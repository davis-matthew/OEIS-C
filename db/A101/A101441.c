/*
 * A101441  n^prime(n+1).
 * https://oeis.org/A101441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^prime(n+1) (prime(1)=2, prime(2)=3, ...)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Simple sieve: enough for n up to about 100000 (prime(100001)≈1.3 M). */
#define SIEVE_LIMIT 2000000U
static uint8_t is_composite[SIEVE_LIMIT + 1];
static unsigned int prime_list[150000];
static int prime_count = 0;

/* Initialise the sieve once. */
static void init_sieve(void) {
    if (prime_count != 0) return;
    /* Mark composites. */
    is_composite[0] = is_composite[1] = 1;
    for (unsigned int i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (unsigned int j = i * i; j <= SIEVE_LIMIT; j += i) {
                is_composite[j] = 1;
            }
        }
    }
    /* Collect primes. */
    for (unsigned int i = 2; i <= SIEVE_LIMIT && prime_count < 150000; ++i) {
        if (!is_composite[i]) {
            prime_list[prime_count++] = i;
        }
    }
}

/* Fast exponentiation: result = base^exp. */
static inline seq_t pow_seq(seq_t base, uint64_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* Public function to compute a(n). */
seq_t a(idx_t n) {
    init_sieve();
    /* For safety, handle out-of-range indices by returning 0. */
    if ((unsigned long)n >= (unsigned long)prime_count) return 0;
    seq_t base = (seq_t)n;
    uint64_t exponent = prime_list[n]; /* prime(n+1) */
    return pow_seq(base, exponent);
}
