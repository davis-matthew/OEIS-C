/*
 * A084310  a(n) = gcd(prime(n)+1, n).
 * https://oeis.org/A084310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(prime(n)+1, n)
 * Range:     u8 n <= 56, u16 n <= 8409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sieve_primes.h"

/*
 * Computes a(n) = gcd(prime(n)+1, n) for n >= 1.
 * The nth prime is stored in a static sieve built once up to 200000,
 * which covers all n up to about 10000 (more than enough for the
 * intended use range).
 */
seq_t a(idx_t n)
{
    static idx_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t sieve_limit = 0;

    /* build sieve once */
    if (prime_list == NULL) {
        sieve_limit = 200000;                /* > 104729 (10000th prime)   */
        sieve_primes(sieve_limit, &prime_list, &prime_count);
    }

    if (n == 0) return 0;                    /* undefined for n=0, but safe */
    idx_t p = prime_list[n - 1];              /* nth prime (1‑based)        */
    seq_t sp = (seq_t)p;
    seq_t sn = (seq_t)n;
    return gcd((seq_t)(sp + 1), sn);
}
