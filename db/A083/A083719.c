/*
 * A083719  a(n) = n * [1 + sum(k=1 to n-1) prime(k)].
 * https://oeis.org/A083719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (1 + ∑_{k=1}^{n-1} prime(k))
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static idx_t *prime_list   = NULL; /* array of the first primes */
static size_t prime_count  = 0;     /* number of primes stored */

/* Ensure that prime_list contains at least `max_index` primes
   (i.e. primes with indices 1 .. max_index). */
static void
ensure_primes(idx_t max_index)
{
    size_t need = (size_t)max_index;
    if (prime_count >= need) return;       /* already have enough */

    /* rough upper bound for the nth prime: 20*n + 100 suffices for n <= 10^5 */
    size_t limit = 20 * need + 100;
    if (limit < 3) limit = 3;

    /* Rebuild a new sieve until we have enough primes. */
    while (1) {
        uint8_t *sieve = (uint8_t *)malloc(limit + 1);
        if (!sieve) return;                 /* out of memory – give up */
        for (size_t i = 0; i <= limit; ++i) sieve[i] = 0;

        for (size_t i = 2; (uint64_t)i * i <= limit; ++i) {
            if (!sieve[i]) {
                for (uint64_t j = (uint64_t)i * i; j <= limit; j += i)
                    sieve[(size_t)j] = 1;
            }
        }

        /* Count primes up to `limit`. */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (!sieve[i]) ++cnt;

        if (cnt >= need) {
            /* Allocate and store the primes. */
            free(prime_list);
            prime_list = (idx_t *)malloc(cnt * sizeof(idx_t));
            if (!prime_list) { free(sieve); return; }

            size_t idx = 0;
            for (size_t i = 2; i <= limit && idx < cnt; ++i)
                if (!sieve[i]) prime_list[idx++] = (idx_t)i;

            prime_count = cnt;
            free(sieve);
            break;
        }

        free(sieve);
        limit *= 2;      /* try a bigger range */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* a(0) = 0 */
    if (n == 1) return 1;   /* a(1) = 1 (sum over empty set) */

    idx_t need = n - 1;     /* number of primes to sum */
    ensure_primes(need);

    seq_t sum = 0;
    for (idx_t i = 0; i < need; ++i)
        sum += (seq_t)prime_list[i];

    seq_t result = (seq_t)n * ((seq_t)sum + 1);
    return result;
}
