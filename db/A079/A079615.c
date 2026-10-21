/*
 * A079615  Product of all distinct prime factors of all composite numbers
 *          between n-th prime and next prime.
 * https://oeis.org/A079615
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = product of distinct primes appearing in any composite
 *            number between the n-th prime and the next prime. The
 *            implementation uses prime factorization of each composite in
 *            that interval and keeps a set of primes already multiplied
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 46, u128 n <= 153,
 *            big any n
 * Helpers:   factor_u64, get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* undefined for n < 2 per OEIS offset */

    /* Get the interval [p+1, q-1] where p = n-th prime, q = (n+1)-th prime */
    idx_t p = get_prime(n);
    idx_t q = get_prime(n + 1);

    seq_t result = 1;

    /* Keep track of primes already multiplied */
    uint64_t seen[128];
    size_t seen_cnt = 0;

    /* Buffers for factorization */
    uint64_t prime_buf[64];
    uint64_t exp_buf[64];   /* not used, but required by factor_u64 */
    size_t cnt;

    for (idx_t m = p + 1; m < q; ++m) {
        /* Skip primes themselves */
        if (isPrimeGeneric((seq_t)m)) continue;

        /* Factor m and get its distinct prime factors */
        factor_u64((uint64_t)m, prime_buf, exp_buf, &cnt);

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t pf = prime_buf[i];
            bool already = false;
            for (size_t j = 0; j < seen_cnt; ++j) {
                if (seen[j] == pf) { already = true; break; }
            }
            if (!already) {
                /* Multiply result by this new prime factor */
                result *= (seq_t)pf;
                seen[seen_cnt++] = pf;
            }
        }
    }

    return result;
}
