/*
 * A100567  Prime-indexed primes as n runs through the integers congruent to 0
 *          or 1 mod 3.
 * https://oeis.org/A100567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime[ prime[ k ] - 1 ] where k is the n‑th integer ≡0 or
 *            1 (mod 3) (1‑based)
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIM 3000000
#define MAX_PRIMES 250000

static bool initialized = false;
static uint32_t primes[MAX_PRIMES];
static idx_t prime_count = 0;

static void init_primes(void)
{
    if (initialized) return;
    uint8_t *composite = (uint8_t*)calloc(LIM + 1, 1);
    if (!composite) return; /* out of memory */

    prime_count = 0;
    for (uint32_t i = 2; i <= LIM; ++i) {
        if (!composite[i]) {
            if (prime_count < MAX_PRIMES) primes[prime_count++] = i;
            if ((uint64_t)i * i <= LIM) {
                for (uint32_t j = i * i; j <= LIM; j += i)
                    composite[j] = 1;
            }
        }
    }
    free(composite);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    if (!initialized) init_primes();
    if (!initialized) return 0;     /* failed to initialize */

    /* kth integer ≡0 or 1 (mod 3) */
    idx_t i = (n - 1) >> 1;
    idx_t k = (n & 1) ? (3 * i + 1) : (3 * i + 3);

    if (k == 0 || k > prime_count) return 0;

    uint32_t pk = primes[(idx_t)k - 1u];
    if (pk == 0 || pk > prime_count) return 0;

    uint32_t result = primes[(idx_t)pk - 1u];
    return (seq_t)result;
}
