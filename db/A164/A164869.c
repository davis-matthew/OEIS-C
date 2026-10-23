/*
 * A164869  n*A027642(n).
 * https://oeis.org/A164869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A027642(n) * where A027642(n) = product of all primes
 *            p such that (p-1) divides n. * For odd n>1 the product is 1, and
 *            a(1)=2. For n=0 a(0)=0
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Minimal sieve for prime tests up to a dynamic limit */
#include <stdlib.h>   /* malloc, free */

static char *prime_sieve = NULL;   /* 0 = composite, 1 = prime */
static idx_t sieve_limit = 0;      /* current maximum index in sieve */

/* Ensure the sieve covers all numbers up to 'up' (inclusive). */
static void ensure_sieve(idx_t up)
{
    if (sieve_limit >= up)
        return;
    /* Allocate new sieve array */
    char *new_sieve = (char *)malloc((size_t)(up + 1));
    if (!new_sieve) { /* fallback: abort if allocation fails */
        return;
    }
    /* Mark all as prime initially */
    for (size_t i = 0; i <= (size_t)up; ++i)
        new_sieve[i] = 1;
    if (up >= 0) new_sieve[0] = 0;
    if (up >= 1) new_sieve[1] = 0;
    /* Sieve of Eratosthenes */
    for (idx_t p = 2; p * p <= up; ++p) {
        if (new_sieve[p]) {
            for (idx_t m = p * p; m <= up; m += p)
                new_sieve[m] = 0;
        }
    }
    /* Replace old sieve */
    if (prime_sieve)
        free(prime_sieve);
    prime_sieve = new_sieve;
    sieve_limit = up;
}

/* Return a(n) = n * A027642(n)                       */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (n == 1)
        return 2;               /* special case */

    if (n & 1) /* odd n > 1: product = 1 */
        return (seq_t)n;

    /* Even n: compute product of primes p with (p-1)|n */
    idx_t up = n + 1;            /* need primes up to n+1 */
    ensure_sieve(up);

    seq_t prod = 1;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t p1 = d + 1;
            if (p1 <= sieve_limit && prime_sieve[p1])
                prod *= (seq_t)p1;

            /* complementary divisor n/d */
            idx_t q = n / d;
            if (q != d) {
                idx_t p2 = q + 1;
                if (p2 <= sieve_limit && prime_sieve[p2])
                    prod *= (seq_t)p2;
            }
        }
    }
    return prod * (seq_t)n;
}
