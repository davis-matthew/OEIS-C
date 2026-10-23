/*
 * A140254  Mobius transform of A014963.
 * https://oeis.org/A140254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} μ(d) * b(n/d) where * b(m) = m if m is a prime
 *            power, * b(m) = 1 otherwise. (b(1)=1) * This yields the correct
 *            Möbius transform of A014963
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   distinct_primes, prime_power_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/distinct_primes.h"
#include "../utils/prime_power_base.h"

/* Recursive enumeration of divisors built from distinct primes only.
 * Only divisors with all exponents 0 or 1 are considered, because μ(d)=0 otherwise.
 */
static void rec_sum(idx_t n, idx_t cur, int mu, const int *primes, int cnt, int pos, seq_t *sum)
{
    if (pos == cnt) {
        idx_t rem = n / cur;                 /* m = n/d */
        seq_t b = prime_power_base(rem);     /* b(m) */
        *sum += (seq_t)mu * b;
        return;
    }
    /* exclude current prime */
    rec_sum(n, cur, mu, primes, cnt, pos + 1, sum);
    /* include current prime */
    rec_sum(n, cur * (idx_t)primes[pos], -mu, primes, cnt, pos + 1, sum);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset starts at 1, behaviour below offset doesn't matter */

    int primes[64];
    int cnt;
    distinct_primes(n, primes, &cnt);

    seq_t result = 0;
    rec_sum(n, (idx_t)1, 1, primes, cnt, 0, &result);
    return result;
}
