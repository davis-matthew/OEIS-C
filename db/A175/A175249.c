/*
 * A175249  Noncomposites (A008578) with nonprime (A018252) subscripts.
 * https://oeis.org/A175249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = noncomposite(nonprime(n))
 * Range:     u8 n <= 39, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   Uses trial division up to sqrt(v).  */
static inline bool is_prime_simple(uint64_t v)
{
    if (v < 2)             return false;
    if (v == 2)            return true;
    if ((v & 1) == 0)      return false;
    for (uint64_t i = 3; i <= v / i; i += 2)
        if (v % i == 0)    return false;
    return true;
}

/* Return the n-th term of A175249 (non‑composites with non‑prime subscripts). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n < 1, defensive */

    idx_t target_nonprime = n;
    idx_t nonprime_cnt = 0;
    idx_t prime_cnt    = 0;
    idx_t x = 1;

    for (;;) {
        if (is_prime_simple((uint64_t)x))
            ++prime_cnt;
        else
            ++nonprime_cnt;

        if (nonprime_cnt == target_nonprime) {
            idx_t bn = x;
            idx_t need_primes = bn - 1;   /* since 1 + π(y) ≥ bn  →  π(y) ≥ bn–1 */

            if (prime_cnt >= need_primes)
                return (seq_t)x;

            for (++x; ; ++x) {           /* search for the required y */
                if (is_prime_simple((uint64_t)x))
                    ++prime_cnt;
                if (prime_cnt >= need_primes)
                    return (seq_t)x;
            }
        }
        ++x;   /* advance to next candidate when not yet found */
    }
}
