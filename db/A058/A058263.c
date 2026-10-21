/*
 * A058263  a(n) = gcd(prime(n) - 1, prime(n+1) - 1).
 * https://oeis.org/A058263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(prime(n)-1 , prime(n+1)-1) (offset = 1)
 * Range:     u8 n <= 53, u16 n <= 7956, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Maximum prime we will generate.  For n up to 10 000, the 10 001st prime is
   104 743; 200 000 suffices for all such cases. */
#define MAX_PRIME_LIMIT 200000
/* Upper bound on the number of primes below MAX_PRIME_LIMIT (≈17984) */
#define MAX_PRIME_COUNT 18000

/* Global storage for the prime table */
static uint8_t sieve[MAX_PRIME_LIMIT + 1];
static uint32_t primes[MAX_PRIME_COUNT];
static size_t prime_cnt = 0;
static int primes_built = 0;

/* Euclidean algorithm for seq_t types */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Build a list of primes up to MAX_PRIME_LIMIT using a simple sieve. */
static void build_primes(void)
{
    if (primes_built) {
        return;
    }
    /* Mark all as prime (0 = prime, 1 = composite) */
    for (size_t i = 0; i <= (size_t)MAX_PRIME_LIMIT; ++i) {
        sieve[i] = 0;
    }
    sieve[0] = sieve[1] = 1;
    for (size_t i = 2; i * i <= (size_t)MAX_PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= (size_t)MAX_PRIME_LIMIT; j += i) {
                sieve[j] = 1;
            }
        }
    }
    /* Extract primes into the array */
    prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)MAX_PRIME_LIMIT; ++i) {
        if (!sieve[i]) {
            if (prime_cnt < (size_t)MAX_PRIME_COUNT) {
                primes[prime_cnt] = (uint32_t)i;
                ++prime_cnt;
            } else {
                /* Exceeded pre‑allocated space – should not happen for
                   n ≤ 10 000. */
                break;
            }
        }
    }
    primes_built = 1;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1; return 0 for other values. */
    if (n < 1) {
        return (seq_t)0;
    }
    /* Ensure the prime table is ready. */
    build_primes();

    /* We need prime(n) and prime(n+1).  Our array is 0‑based, so
       prime(n) -> primes[n-1] and prime(n+1) -> primes[n]. */
    seq_t p1 = (seq_t)primes[n - 1] - (seq_t)1;
    seq_t p2 = (seq_t)primes[n]     - (seq_t)1;

    return gcd_seq_t(p1, p2);
}
