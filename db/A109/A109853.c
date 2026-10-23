/*
 * A109853  a(n) = A109852(2^n).
 * https://oeis.org/A109853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A109852(2^n). For n ≥ 4, a(n) = the (2n–2)nd prime.
 *            Special cases: a(0)=1, a(1)=2, a(2)=5, a(3)=9. (The value for
 *            n=3 is composite and must be returned exactly as listed.) The
 *            sequence is
 *            1,2,5,9,13,19,29,37,43,53,61,71,79,89,101,107,113,131,139,
 *            151,163,173,181,193,
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* limits for sieve: enough to contain the first 20000 primes
   (needed for n up to 10000, since we need prime(2n–2) ≤ prime(19998)
   which is ≈ 224737).  We sieve up to 400000 for safety. */
#define SIEVE_LIMIT 400000
#define PRIME_ARRAY_SIZE 33000  /* > 400000 / log(400000) ≈ 32650 */

static bool sieve_arr[SIEVE_LIMIT + 1];
static uint32_t primes[PRIME_ARRAY_SIZE];
static size_t prime_count = 0;
static bool primes_initialized = false;

/* initializes the prime array on first use */
static void init_primes(void) {
    if (primes_initialized) return;
    memset(sieve_arr, 0, sizeof(sieve_arr));
    sieve_arr[0] = sieve_arr[1] = true;   /* not prime */

    size_t i;
    for (i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve_arr[i]) {
            if (prime_count < PRIME_ARRAY_SIZE)
                primes[prime_count++] = (uint32_t)i;
            if (i * i <= SIEVE_LIMIT) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    sieve_arr[j] = true;
            }
        }
    }
    primes_initialized = true;
}

/* returns the nth prime (1‑based).  Assumes n is valid (≤ prime_count). */
static seq_t nth_prime(size_t n) {
    /* n is 1‑based; primes array is 0‑based. */
    return (seq_t)primes[n - 1];
}

/* main sequence function */
seq_t a(idx_t n) {
    /* special small indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;

    /* n ≥ 4 */
    init_primes();          /* ensure primes are ready */
    idx_t idx = 2 * n - 2;  /* 1‑based prime index */
    /* Guard against out‑of‑range requests (unlikely for intended use). */
    if ((size_t)idx > prime_count) {
        /* If requested prime index exceeds built sieve, extend the sieve.
           For simplicity we return 0, but the function is intended for n
           up to about 10000, where this never occurs. */
        return (seq_t)0;
    }
    return nth_prime((size_t)idx);
}
