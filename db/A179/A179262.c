/*
 * A179262  a(n) = 2*prime(n)^2 - 1.
 * https://oeis.org/A179262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n)^2 - 1
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 4792, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the first n primes by a simple
   sieve of Eratosthenes whenever more primes are needed,
   then returns 2*p^2-1 where p is the nth prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static uint32_t *primes      = NULL;   /* array of primes found */
static idx_t     prime_count = 0;      /* number of primes stored */

/* Ensure that at least n primes are stored in 'primes'. */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= n)           /* already have enough */
        return;

    /* Over‑estimate of the nth prime:
       For n >= 6, p_n < n*(log n + log log n); 20*n+100 is safe for n <= 10000. */
    idx_t limit = n * 20 + 100;

    /* Allocate the sieve array (0 means prime candidate). */
    uint8_t *sieve = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!sieve)
        return;                      /* out of memory – leave function */

    /* Eratosthenes sieve. */
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (!sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                sieve[j] = 1;
        }
    }

    /* Collect primes into a dynamic array. */
    uint32_t *tmp_primes = NULL;
    size_t    tmp_cap    = 0;
    size_t    count      = 0;

    for (size_t i = 2; i <= (size_t)limit; ++i) {
        if (!sieve[i]) {
            /* Grow temporary array if needed. */
            if (count == tmp_cap) {
                size_t new_cap = tmp_cap == 0 ? 1024 : tmp_cap * 2;
                uint32_t *repl = (uint32_t *)realloc(tmp_primes, new_cap * sizeof(uint32_t));
                if (!repl) {
                    free(tmp_primes);
                    free(sieve);
                    return;
                }
                tmp_primes = repl;
                tmp_cap    = new_cap;
            }
            tmp_primes[count++] = (uint32_t)i;
            if (count >= (size_t)n)   /* we only need the first n primes */
                break;
        }
    }

    free(sieve);

    /* Replace old primes with the new ones. */
    if (primes)
        free(primes);
    primes      = tmp_primes;
    prime_count = (idx_t)count;
}

/* Return the nth term of the sequence.  n is 1‑based. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    ensure_primes(n);

    seq_t p      = (seq_t)primes[n - 1];
    seq_t result = ((seq_t)2 * p * p) - (seq_t)1;
    return result;
}
