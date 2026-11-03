/*
 * A077149  a(1) =3. For n>1, a(n) = smallest k such that there are n numbers
 *          m not relatively prime to n in range n < m < k.
 * https://oeis.org/A077149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined as the smallest integer k>n such that there are
 *            exactly n distinct integers m with n < m < k and gcd(m,n) > 1
 *            (i.e. m is not coprime to n). The algorithm factors n into its
 *            distinct prime divisors, uses inclusion–exclusion to count how
 *            many integers ≤x are divisible by at least on...
 * Range:     u8 n <= 16, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Count integers ≤x divisible by at least one prime in primes[0..cnt-1]. */
static seq_t count_divisible(idx_t x, const seq_t *primes, size_t cnt) {
    seq_t total = 0;
    idx_t subsets = ((idx_t)1 << cnt) - 1;
    for (idx_t mask = 1; mask <= subsets; ++mask) {
        seq_t prod = 1;
        size_t bits = 0;
        for (idx_t i = 0; i < cnt; ++i) {
            if (mask & ((idx_t)1 << i)) {
                prod *= primes[i];
                ++bits;
            }
        }
        if (bits & 1) {
            total += x / prod;
        } else {
            total -= x / prod;
        }
    }
    return total;
}

/* Return the distinct prime factors of n in pf[], setting *cnt. */
static void distinct_prime_factors(idx_t n, seq_t *pf, size_t *cnt) {
    size_t c = 0;
    idx_t x = n;
    for (idx_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            pf[c++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) pf[c++] = x;
    *cnt = c;
}

seq_t a(idx_t n) {
    if (n == (idx_t)1) return (seq_t)3;          /* defined by OEIS */

    seq_t primes[6];
    size_t np;
    distinct_prime_factors(n, primes, &np);

    seq_t count_n = count_divisible(n, primes, np);
    seq_t target  = count_n + n;                      /* need C(k-1) = target */

    idx_t low  = n + 1;
    idx_t high = (idx_t)(((seq_t)n * ((seq_t)n + 1u)) + 1u); /* safe upper bound (prime case) */
    if (high < low) high = low;

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t cmid = count_divisible(mid, primes, np);
        if (cmid >= target) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    return (seq_t)(low + 1);                         /* k = (k-1)+1 */
}
