/*
 * A120293  Absolute value of numerator of determinant of n X n matrix with
 *          M(i,j) = (i+1)/(i+2) if i=j otherwise 1.
 * https://oeis.org/A120293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = (n^2 + 5n - 2) / gcd(n^2 + 5n - 2, (n+2)!)
 * Range:     u8 n <= 14, u16 n <= 258, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRIME_LIMIT 50000        /* enough for n up to 50 000 */

static uint64_t primes_buf[MAX_PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_inited = false;

/* Simple sieve of Eratosthenes.  It is run only once. */
static void init_primes(void)
{
    if (primes_inited)
        return;

    bool composite[MAX_PRIME_LIMIT + 1];
    memset(composite, 0, sizeof(composite));

    for (idx_t i = 2; i <= MAX_PRIME_LIMIT; ++i) {
        if (!composite[i]) {
            primes_buf[prime_count++] = (uint64_t)i;
            if ((idx_t)i * (idx_t)i <= MAX_PRIME_LIMIT) {
                for (idx_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                    composite[j] = true;
            }
        }
    }
    primes_inited = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    seq_t n_seq = (seq_t)n;
    seq_t A = n_seq * n_seq + (seq_t)5 * n_seq - (seq_t)2;   /* numerator before reduction */
    seq_t result = A;
    seq_t n_plus_2 = n_seq + (seq_t)2;

    /* Iterate over primes not exceeding n+2; these are the only primes
       that can divide the factorial (n+2)!. */
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)primes_buf[i];
        if (p > n_plus_2)
            break;

        /* Compute exponent of p in A */
        seq_t expA = 0;
        seq_t tempA = A;
        while (tempA % p == 0) {
            tempA /= p;
            ++expA;
        }
        if (expA == 0)
            continue;

        /* Compute exponent of p in (n+2)!  */
        seq_t expB = 0;
        seq_t pw = p;
        while (pw <= n_plus_2) {
            expB += n_plus_2 / pw;
            if (pw > n_plus_2 / p)   /* prevent overflow of pw *= p */
                break;
            pw *= p;
        }

        /* Exponent to remove from numerator */
        seq_t exp_min = expA < expB ? expA : expB;
        seq_t powp = 1;
        for (seq_t k = 0; k < exp_min; ++k)
            powp *= p;

        result /= powp;
    }

    return result;
}
