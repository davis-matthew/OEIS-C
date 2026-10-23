/*
 * A129543  Gray code ordering of the prime numbers.
 * https://oeis.org/A129543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(1 + A003188(n))
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Pre‑compute primes up to a fixed limit.  The 20000th prime is
   224737, so 300000 is more than ample for n ≤ 10000. */
static const unsigned MAX_LIMIT = 300000;
static unsigned *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (prime_list != NULL)
        return;                      /* already initialized */

    size_t limit = MAX_LIMIT;
    unsigned char *sieve = (unsigned char *)calloc(limit + 1, 1);
    if (!sieve)
        exit(1);

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Count primes. */
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            ++prime_count;

    prime_list = (unsigned *)malloc(prime_count * sizeof(unsigned));
    if (!prime_list)
        exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!sieve[i])
            prime_list[idx++] = (unsigned)i;

    free(sieve);
}

seq_t a(idx_t n)
{
    init_primes();

    idx_t gray = n ^ (n >> 1);   /* A003188(n) */
    idx_t idx  = gray + 1;       /* 1‑based prime index */

    /* idx is guaranteed to fit within the pre‑computed list for
       n ≤ 10000 with the chosen limit. */
    return (seq_t)prime_list[(size_t)(idx - 1)];
}
