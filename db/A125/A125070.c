/*
 * A125070  a(n) = number of nonzero exponents in the prime factorization of n
 *          which are not primes.
 * https://oeis.org/A125070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of non‑zero exponents in the prime factorisation
 *            of n that are not prime numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_prime_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/build_spf.h"
#include "../utils/is_prime_exponent.h"

static idx_t *spf = NULL;    /* smallest prime factor array */
static idx_t spf_limit = 0;  /* last limit for which spf was built */

/* build spf array up to at least max_n */
static void ensure_spf(idx_t max_n)
{
    if (max_n <= spf_limit) return;
    if (spf != NULL) free(spf);

    build_spf(max_n, &spf);
    spf_limit = max_n;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;         /* n=1 has no exponents */

    /* prepare spf array up to n */
    ensure_spf(n);

    uint64_t nn = (uint64_t)n;
    seq_t count = 0;

    while (nn > 1) {
        uint64_t p = (uint64_t)spf[nn];
        uint64_t e = 0;
        while (nn % p == 0) {
            nn /= p;
            ++e;
        }
        if (!is_prime_exponent(e))
            ++count;
    }

    return count;
}
