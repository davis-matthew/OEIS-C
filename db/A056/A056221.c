/*
 * A056221  Image of primes (A000040) under "little Hankel" transform that
 *          sends [c_0, c_1, ...] to [d_0, d_1, ...] where d_n = c_n^2 -
 *          c_{n+1}*c_{n-1}.
 * https://oeis.org/A056221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = prime(n+1)^2 - prime(n) * prime(n+2) (primes are
 *            1‑indexed)
 * Range:     i8 n <= 8, i16 n <= 215, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define PRIME_LIMIT 200000   /* enough to contain the first 10000 primes */

static unsigned char comp[PRIME_LIMIT + 1];  /* 0 = prime, 1 = composite */
static int primes[PRIME_LIMIT];              /* primes[0] = 2 */
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;

    /* Sieve of Eratosthenes */
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                comp[j] = 1;
        }
    }
    for (int i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = i;
        }
    }
    primes_inited = true;
}

seq_t a(idx_t n)
{
    /* No assumptions about order or range of n, n >= 1 */
    init_primes();

    /* Ensure we have enough primes; the limit chosen is safe for n up to ~10000 */
    if (n + 2 > primes_cnt) {
        /* Should not happen for the intended range */
        return 0;
    }

    seq_t p_n   = (seq_t)primes[n - 1];
    seq_t p_n1  = (seq_t)primes[n];
    seq_t p_n2  = (seq_t)primes[n + 1];

    return p_n1 * p_n1 - p_n * p_n2;
}
