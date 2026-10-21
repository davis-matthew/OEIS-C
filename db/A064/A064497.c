/*
 * A064497  a(n) = prime(n) * Fibonacci(n).
 * https://oeis.org/A064497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) * Fibonacci(n) (1‑based index)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 81, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static idx_t prime_cache_size = 0;
static uint64_t *prime_cache = NULL;

static idx_t fib_cache_size = 0;
static seq_t *fib_cache = NULL;

/* Generate all primes up to the given count (1‑based). */
static void ensure_primes(idx_t need)
{
    if (prime_cache_size >= need)
        return;

    /* Grow the prime array to the required size. */
    uint64_t *tmp = (uint64_t *)realloc(prime_cache,
                                        need * sizeof(uint64_t));
    if (!tmp)
        exit(1);            /* fatal error */
    prime_cache = tmp;

    /* Find primes until we have enough. */
    for (idx_t i = prime_cache_size; i < need; ++i) {
        uint64_t cand = (i == 0) ? 2 : prime_cache[i - 1] + 1;
        while (1) {
            int composite = 0;
            for (idx_t j = 0; j < prime_cache_size; ++j) {
                uint64_t p = prime_cache[j];
                if (p * p > cand)
                    break;
                if (cand % p == 0) {
                    composite = 1;
                    break;
                }
            }
            if (!composite) {
                prime_cache[prime_cache_size++] = cand;
                break;
            }
            ++cand;
        }
    }
}

/* Generate Fibonacci numbers (1‑based) up to the given count. */
static void ensure_fibs(idx_t need)
{
    if (fib_cache_size >= need)
        return;

    seq_t *tmp = (seq_t *)realloc(fib_cache,
                                  need * sizeof(seq_t));
    if (!tmp)
        exit(1);            /* fatal error */
    fib_cache = tmp;

    for (idx_t i = fib_cache_size; i < need; ++i) {
        if (i == 0) {
            fib_cache[0] = (seq_t)1;
        } else if (i == 1) {
            fib_cache[1] = (seq_t)1;
        } else {
            fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
        }
        fib_cache_size = i + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n<1, but safe return */

    ensure_primes(n);
    ensure_fibs(n);

    seq_t prime = (seq_t)prime_cache[n - 1];   /* primes are 0‑based */
    seq_t fib   = fib_cache[n - 1];

    return prime * fib;
}
