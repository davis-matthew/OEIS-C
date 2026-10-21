/*
 * A077679  Squarefree numbers beginning with 3.
 * https://oeis.org/A077679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = nth squarefree positive integer whose decimal
 *            representation * begins with the digit 3. The function generates
 *            successively the ranges * [3·10^k, 4·10^k‑1] for k = 0, 1, 2 …
 *            and tests each number in these * ranges for the square‑free
 *            property. The square‑free test is performed by * checkin...
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Pre‑compute primes up to a fixed limit (here 100 000).  This covers all
 * squares needed for numbers involved in the first 10 000 sequence terms. */
#define PRIME_LIMIT 100000U
static uint64_t primes[PRIME_LIMIT / 10];   /* sufficient storage */
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    size_t i, p;
    bool *is_prime = (bool *)malloc(PRIME_LIMIT + 1);
    if (!is_prime) exit(1);
    for (i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (i = p * p; i <= PRIME_LIMIT; i += p) is_prime[i] = false;
        }
    }
    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = i;
    }
    free(is_prime);
    primes_initialized = true;
}

/* Test if x is square‑free by checking divisibility by p² for all primes
 * p with p² ≤ x. */
static bool is_squarefree(uint64_t x)
{
    size_t i;
    for (i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t p2 = p * p;
        if (p2 > x) break;
        if (x % p2 == 0) return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!primes_initialized) init_primes();

    uint64_t power10 = 1;        /* 10^k */
    uint64_t counter = 0;        /* number of terms found */

    for (;;) {
        uint64_t start = 3 * power10;
        uint64_t end   = 4 * power10 - 1;

        for (uint64_t x = start; x <= end; ++x) {
            if (is_squarefree(x)) {
                ++counter;
                if (counter == n) {
                    return (seq_t)x;
                }
            }
        }

        /* Move to the next power of ten, careful of overflow */
        if (power10 > UINT64_MAX / 10) break;  /* safety, though not needed */
        power10 *= 10;
    }

    /* Should never reach here for valid inputs */
    return (seq_t)0;
}
