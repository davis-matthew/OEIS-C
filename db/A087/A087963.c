/*
 * A087963  Exponent of highest power of 2 dividing 3*prime(n)+1.
 * https://oeis.org/A087963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of highest power of 2 dividing 3*prime(n)+1
 *            (OEIS A087963)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*--- Prime generation -----------------------------------------------------*/
static const uint32_t MAX_SIEVE = 200000;          /* enough for first 20001 primes */
static uint64_t *prime_array = NULL;
static size_t prime_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    primes_initialized = true;

    /* Allocate temporary sieve */
    bool *sieve = (bool *)malloc(MAX_SIEVE + 1);
    if (!sieve) exit(1);
    memset(sieve, 0, MAX_SIEVE + 1);

    prime_array = (uint64_t *)malloc(MAX_SIEVE / 2 * sizeof(uint64_t));
    if (!prime_array) exit(1);
    prime_count = 0;

    for (uint32_t i = 2; i <= MAX_SIEVE; ++i) {
        if (!sieve[i]) {
            if (prime_count >= MAX_SIEVE / 2) break;
            prime_array[prime_count++] = i;
            if ((uint64_t)i * i <= MAX_SIEVE) {
                for (uint32_t j = i * i; j <= MAX_SIEVE; j += i)
                    sieve[j] = true;
            }
        }
    }
    free(sieve);
}

/*--- Sequence -------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1; undefined below 1 */

    init_primes();
    if (n - 1 >= prime_count) {
        /* Safe guard – should never happen for the intended limits. */
        return 0;
    }

    uint64_t p = prime_array[n - 1];
    seq_t val = (seq_t)3 * p + 1;

    seq_t cnt = 0;
    while ((val & 1) == 0) {
        val >>= 1;
        cnt++;
    }
    return cnt;
}
