/*
 * A138183  Smallest Fibonacci number not less than the n-th prime.
 * https://oeis.org/A138183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest Fibonacci number not less than the n‑th prime
 * Range:     u8 n <= 51, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static seq_t *fibs      = NULL;   /* Fibonacci numbers */
static idx_t  fib_last  = 0;      /* highest index currently computed */
static size_t fib_cap   = 0;      /* allocated capacity of fibs */

static seq_t *primes    = NULL;   /* cached primes (1‑based, stored 0‑based) */
static idx_t  prime_cnt = 0;      /* number of primes stored */
static size_t prime_cap = 0;      /* allocated capacity of primes */

/* Ensure fibs[0 .. up_to] are computed and allocated. */
static void ensure_fib(idx_t up_to)
{
    if (fibs == NULL) {
        fib_cap = 4;
        fibs = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fibs[0] = 0;
        fibs[1] = 1;
        fib_last = 1;
    }
    while (fib_last < up_to) {
        /* grow array if needed */
        if ((size_t)(fib_last + 1) >= fib_cap) {
            size_t new_cap = fib_cap * 2;
            seq_t *tmp = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
            fibs = tmp;
            fib_cap = new_cap;
        }
        /* compute next Fibonacci number */
        fibs[fib_last + 1] = fibs[fib_last] + fibs[fib_last - 1];
        fib_last++;
    }
}

/* Return the smallest Fibonacci value ≥ val. */
static seq_t fib_ge(seq_t val)
{
    /* initialize fibs if needed (used when n == 1). */
    if (fibs == NULL) {
        fib_cap = 4;
        fibs = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        fibs[0] = 0;
        fibs[1] = 1;
        fib_last = 1;
    }

    idx_t i = 0;
    while (1) {
        if (i > fib_last)           /* need more Fibonacci numbers */
            ensure_fib(i);
        if (fibs[i] >= val)
            return fibs[i];
        i++;
    }
}

/* Cache enough primes so that prime_cnt >= n. */
static void ensure_prime_upto_idx(idx_t n)
{
    if (primes == NULL) {
        prime_cap = 2000;
        primes = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        prime_cnt = 0;
    }

    idx_t candidate = (prime_cnt == 0) ? 2 : (idx_t)primes[prime_cnt - 1] + 1;
    while (prime_cnt < n) {
        if (is_prime(candidate)) {
            if ((size_t)prime_cnt >= prime_cap) {
                size_t new_cap = prime_cap * 2;
                seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = (seq_t)candidate;
        }
        candidate++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1; behaviour for n<1 is irrelevant */
        return 0;

    ensure_prime_upto_idx(n);
    seq_t p = primes[n - 1];   /* primes are stored 0‑based */
    return fib_ge(p);
}
