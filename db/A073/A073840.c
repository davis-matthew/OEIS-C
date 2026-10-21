/*
 * A073840  Product of the composite numbers between n and 2n (both
 *          inclusive).
 * https://oeis.org/A073840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product of composite numbers between n and 2n (inclusive)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: sieve of Eratosthenes, cached up to the largest limit requested. */
static uint8_t *prime_flag = NULL;   /* 0 = composite, 1 = prime */
static idx_t prime_limit = 0;

static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit)
        return;               /* already have a sieve up to this limit */

    if (prime_flag)
        free(prime_flag);

    prime_flag = (uint8_t *)malloc((size_t)(limit + 1));
    if (!prime_flag)          /* out of memory – caller will fail silently */
        return;

    for (idx_t i = 0; i <= limit; ++i)
        prime_flag[i] = 1;    /* assume prime */

    if (limit >= 0)
        prime_flag[0] = 0;
    if (limit >= 1)
        prime_flag[1] = 0;

    for (idx_t i = 2; (i * i) <= limit; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                prime_flag[j] = 0;
        }
    }
    prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 1;                  /* offset is 1; undefined behaviour for n < 1 */

    idx_t limit = n * 2;
    ensure_prime_sieve(limit);

    seq_t res = 1;
    for (idx_t k = n; k <= limit; ++k) {
        if (k > 1 && !prime_flag[k])      /* composite number */
            res = res * (seq_t)k;
    }
    return res;
}
