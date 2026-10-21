/*
 * A061150  a(n) = Sum_{d|n} d*prime(d).
 * https://oeis.org/A061150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d * prime(d)
 * Range:     u8 n <= 9, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a simple sieve that generates the first n primes on demand. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>

/*  Static storage for the prime list.  */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;

/*  Ensure that prime_list contains at least `needed` primes.  */
static void ensure_primes(idx_t needed)
{
    if (needed <= prime_count)
        return;                     /* Already have enough primes. */

    /*  Estimate an upper bound for the `needed`‑th prime.  */
    idx_t limit;
    if (needed < 6) {
        limit = 15;                  /* small safeguard for tiny n */
    } else {
        double n = (double)needed;
        double bound = n * (log(n) + log(log(n))) + 3.0;
        limit = (idx_t)bound + 10;
    }

    /*  Allocate / reallocate the prime buffer.  */
    prime_list = (uint64_t *)realloc(prime_list, needed * sizeof(uint64_t));
    if (!prime_list)
        exit(1);                     /* allocation failure */

    /*  Sieve of Eratosthenes up to `limit`.  */
    bool *is_comp = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_comp)
        exit(1);
    for (idx_t i = 2; i <= limit; ++i)
        is_comp[i] = false;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit && cnt < needed; ++i) {
        if (!is_comp[i]) {
            prime_list[cnt++] = i;
            if ((uint64_t)i * (uint64_t)i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    prime_count = cnt;
}

/*  Compute a(n) = sum_{d|n} d * prime(d).  */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;          /* safety for n < 1 */

    ensure_primes(n);             /* need primes up to index n */

    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)1; i * i <= n; ++i) {
        if (n % i == (idx_t)0) {
            /* divisor i */
            seq_t contrib1 = (seq_t)i * (seq_t)prime_list[i - 1];
            sum += contrib1;
            idx_t q = n / i;
            if (q != i) {
                /* paired divisor n/i */
                seq_t contrib2 = (seq_t)q * (seq_t)prime_list[q - 1];
                sum += contrib2;
            }
        }
    }
    return sum;
}
