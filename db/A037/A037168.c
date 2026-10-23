/*
 * A037168  a(n) = 2*prime(n) - 2.
 * https://oeis.org/A037168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n) - 2
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* uint32_t */
#include <stdbool.h>  /* bool */

static uint32_t *prime_buf   = NULL;  /* array of primes */
static idx_t     prime_cnt   = 0;      /* number of primes stored */
static bool      primes_init = false; /* flag */

/* Build the prime sieve once (sufficient for n ≤ 10000). */
static void init_primes(void)
{
    /* Upper bound > 2 * prime(10000) */
    const idx_t limit = 200000;
    char *sieve = (char *)malloc((limit + 1) * sizeof(char));
    if (!sieve) return;                     /* Out of memory → leave uninitialized */

    for (idx_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = 0;
        }
    }

    /* Count primes */
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++prime_cnt;

    /* Store primes in an array */
    prime_buf = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_buf) { free(sieve); return; }

    idx_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (sieve[i]) prime_buf[pos++] = (uint32_t)i;
    }

    free(sieve);
    primes_init = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined below the offset */

    if (!primes_init) init_primes();
    if (!primes_init || n > prime_cnt) return (seq_t)0;

    seq_t p = (seq_t)prime_buf[n - 1];
    return (seq_t)2 * p - (seq_t)2;
}
