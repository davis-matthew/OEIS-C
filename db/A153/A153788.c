/*
 * A153788  Number of proper divisors of the Catalan number A000108(n).
 * https://oeis.org/A153788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(C_n) - 1 where C_n = (2n)!/(n!(n+1)!) and d(x) is the
 *            number of positive divisors of x
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 101, u64 n <= 227,
 *            u128 n <= 549, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;     /* array of primes up to prime_limit   */
static size_t prime_count = 0;      /* number of primes in the array        */
static idx_t   prime_limit = 0;     /* largest n for which primes are valid */

/* Ensure we have all primes up to 2*n.  Rebuild the table if necessary. */
static void ensure_primes(idx_t n)
{
    idx_t needed = 2 * n;
    if (needed <= prime_limit) return;
    if (primes) free(primes);
    build_primes_upto(needed, &primes, &prime_count);
    prime_limit = needed;
}

/* Number of proper divisors of the Catalan number C_n. */
seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return (seq_t)0;   /* C_0 = C_1 = 1, no proper divisors */

    ensure_primes(n);

    seq_t result = (seq_t)1;   /* product of (exponent+1) over all primes */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p > 2 * n) break;          /* primes beyond 2n cannot contribute */

        idx_t exp = 0;
        idx_t power = p;
        while (power <= 2 * n) {
            exp += (2 * n) / power;
            exp -= n / power;
            exp -= (n + 1) / power;

            /* stop before multiplication would overflow or exceed 2*n */
            if (power > (2 * n) / p) break;
            power *= p;
        }

        if (exp > 0)
            result *= (seq_t)(exp + 1);
    }

    return result - (seq_t)1;   /* subtract 1 to exclude the number itself */
}
