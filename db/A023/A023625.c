/*
 * A023625  Convolution of Lucas numbers and primes.
 * https://oeis.org/A023625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} L(k) * p_{n+1-k} L(0)=2, L(1)=1,
 *            L(k)=L(k-1)+L(k-2) p_1=2, p_2=3, … (prime sequence)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code below works both for C and C++ builds and
   handles arbitrary‑precision seq_t as long as the standard
   allocation functions are available. */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *prime_cache = NULL;    /* 1‑indexed array of primes   */
static size_t prime_cap   = 0;          /* allocated size (includes index 0) */
static size_t prime_count = 0;          /* number of primes stored */

/* Build or enlarge the prime table up to the nth prime. */
static void ensure_primes(size_t n)
{
    if (prime_cap >= n + 1 && prime_count >= n)
        return;                        /* already enough */

    /* Free old table if it exists */
    if (prime_cache) {
        free(prime_cache);
        prime_cache = NULL;
        prime_cap   = 0;
        prime_count = 0;
    }

    /* Allocate new cache (1‑indexed; index 0 unused) */
    prime_cache = (uint64_t *)calloc(n + 1, sizeof(uint64_t));
    if (!prime_cache)                 /* allocation failure */
        return;

    /* Simple upper bound for the n‑th prime: 20*n+100
       which is well above the true value for n ≤ 10 000. */
    size_t limit = n * 20 + 100;
    if (limit < 200)
        limit = 200;

    unsigned char *sieve = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    if (!sieve) {
        free(prime_cache);
        prime_cache = NULL;
        return;
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= limit && cnt < n; ++i) {
        if (!sieve[i]) {
            ++cnt;
            prime_cache[cnt] = i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 1;
            }
        }
    }
    free(sieve);
    prime_cap   = n + 1;
    prime_count = cnt;
}

/* Computes the n‑th term of A023625. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) is unused but defined to be 0 */

    /* Ensure we have the first n primes. */
    ensure_primes((size_t)n);

    seq_t sum = 0;
    /* Lucas numbers: start with L(0)=2, L(1)=1 */
    seq_t L_prev2 = (seq_t)2;   /* L(0) */
    seq_t L_prev1 = (seq_t)1;   /* L(1) */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t L_k   = L_prev1;                                  /* L(k) */
        seq_t prod  = L_k * (seq_t)prime_cache[(size_t)(n + 1 - k)];
        sum += prod;

        /* Advance Lucas numbers for next iteration. */
        seq_t L_next = L_prev2 + L_prev1;
        L_prev2 = L_prev1;
        L_prev1 = L_next;
    }
    return sum;
}
