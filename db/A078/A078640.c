/*
 * A078640  Number of numbers between 1 and n-1 inclusive that are coprime to
 *          n*(n+1)*(n+2).
 * https://oeis.org/A078640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A078640
    a(n) = number of integers k with 1 ≤ k < n and gcd(k, n(n+1)(n+2)) = 1
    Implementation is by inclusion–exclusion over the distinct prime
    divisors of n, n+1 and n+2.   */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Collect distinct prime factors of a 64‑bit value into buf.  */
static void factor_uint64(uint64_t val, uint64_t *buf, size_t *cnt)
{
    /* handle factor 2 separately */
    if ((val & 1ULL) == 0) {
        buf[(*cnt)++] = 2ULL;
        while ((val & 1ULL) == 0) val >>= 1;
    }
    for (uint64_t p = 3; p * p <= val; p += 2) {
        if (val % p == 0) {
            buf[(*cnt)++] = p;
            while (val % p == 0) val /= p;
        }
    }
    if (val > 1) {
        buf[(*cnt)++] = val;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety, not needed for the OEIS offset */

    /* collect unique prime factors of n, n+1, n+2 */
    uint64_t primes[64];
    size_t   np = 0;

    factor_uint64((uint64_t)n,     primes, &np);
    factor_uint64((uint64_t)(n + 1), primes, &np);
    factor_uint64((uint64_t)(n + 2), primes, &np);

    /* Remove duplicates (factor_uint64 may insert duplicates when the same
       prime appears in two of the three numbers). */
    size_t uniq = 0;
    for (size_t i = 0; i < np; ++i) {
        bool present = false;
        for (size_t j = 0; j < uniq; ++j)
            if (primes[i] == primes[j]) { present = true; break; }
        if (!present) primes[uniq++] = primes[i];
    }
    np = uniq;

    seq_t limit = (seq_t)(n - 1);      /* numbers 1 … n-1 */
    seq_t result = limit;             /* start with all numbers */

    if (np == 0) return result;       /* n = 1 gives no numbers in the range */

    /* Inclusion–exclusion over all non‑empty subsets of the prime set */
    for (uint64_t mask = 1; mask < (1ULL << np); ++mask) {
        seq_t prod = 1;
        size_t bits = 0;
        bool overflow = false;
        for (size_t i = 0; i < np; ++i) {
            if (mask & (1ULL << i)) {
                ++bits;
                /* avoid overflow: if prod > limit / p then prod > limit */
                if (!overflow && prod <= limit / (seq_t)primes[i]) {
                    prod *= (seq_t)primes[i];
                } else {
                    overflow = true;
                    prod = limit + 1;   /* any value > limit is enough */
                }
            }
        }
        seq_t cnt = prod > limit ? 0 : limit / prod;
        if (bits & 1) result -= cnt; else result += cnt;
    }

    return result;
}
