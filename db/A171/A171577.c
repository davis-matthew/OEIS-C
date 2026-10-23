/*
 * A171577  0, 1 and primes > 3.
 * https://oeis.org/A171577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(1)=0, a(2)=1, a(n)=the n‑th prime for n≥3 (primes >3)
 * Range:     u8 n <= 54, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static uint64_t *prime_cache = NULL;   /* 0‑based array of primes */
static size_t prime_count   = 0;       /* number of primes stored */

/* Generate at least the first `need` primes. `need` ≥ 1. */
static void ensure_primes(idx_t need)
{
    if ((idx_t)prime_count >= need)
        return;

    /* Estimate an upper bound for the `need`‑th prime (Rosser's theorem) */
    uint64_t limit;
    if (need < 6)
        limit = 15;                     /* 3rd prime is 5, 4th 7, 5th 11 */
    else {
        double n = (double)need;
        limit = (uint64_t)(n * (log(n) + log(log(n)))) + 10;
    }

    while (1) {
        /* Allocate a flag array for composites */
        bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
        if (!is_comp)        /* out of memory – give up */
            return;

        uint64_t sqrt_lim = (uint64_t)sqrt((double)limit);
        for (uint64_t i = 2; i <= sqrt_lim; ++i) {
            if (!is_comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }

        /* Count primes up to the limit */
        size_t cnt = 0;
        for (uint64_t i = 2; i <= limit; ++i)
            if (!is_comp[i]) ++cnt;

        if (cnt >= (size_t)need) {
            /* Extract the primes */
            uint64_t *arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
            if (!arr) {            /* out of memory – give up */
                free(is_comp);
                return;
            }
            size_t idx = 0;
            for (uint64_t i = 2; i <= limit; ++i)
                if (!is_comp[i]) arr[idx++] = i;

            /* Replace old cache */
            free(prime_cache);
            prime_cache = arr;
            prime_count = cnt;
            free(is_comp);
            break;
        }

        free(is_comp);
        limit *= 2;                  /* increase limit and try again */
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Ensure we have at least n primes */
    ensure_primes(n);

    /* prime_cache is 0‑based, nth prime is at index n‑1 */
    if ((size_t)prime_count > (size_t)(n - 1))
        return (seq_t)prime_cache[n - 1];
    /* In case the sieve failed to yield enough primes, return 0 */
    return (seq_t)0;
}
