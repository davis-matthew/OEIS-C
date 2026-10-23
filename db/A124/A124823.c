/*
 * A124823  a(n) = n-th integer from among those positive integers which are
 *          coprime to n(n+1)/2.
 * https://oeis.org/A124823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n-th integer from among those positive integers which are
 *            coprime to n(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 1346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Count how many integers in [1,x] are coprime to the number whose
   distinct prime factors are listed in PRIMES[0..PrimesCount-1].
   Inclusion–exclusion is used. */
static idx_t count_coprime(idx_t x, const seq_t *primes, size_t PrimesCount)
{
    if (PrimesCount == 0) return x;
    idx_t cnt = x;
    size_t subsets = 1ULL << PrimesCount;
    for (size_t mask = 1; mask < subsets; ++mask) {
        seq_t prod = 1;
        unsigned bits = 0;
        for (size_t j = 0; j < PrimesCount; ++j) {
            if (mask & (1ULL << j)) {
                ++bits;
                prod *= primes[j];
                if (prod > x) break; /* product too large, term will be 0 */
            }
        }
        if (prod > x) continue;
        idx_t term = x / (idx_t)prod;
        if (bits & 1) cnt -= term;
        else cnt += term;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* Compute T = n*(n+1)/2 using seq_t arithmetic */
    seq_t T = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Factor T into distinct primes (trial division) */
    seq_t primes[64];
    size_t np = 0;
    idx_t d = 2;
    seq_t tmp = T;
    while ((seq_t)d * d <= tmp) {
        if (tmp % d == 0) {
            primes[np++] = d;
            while (tmp % d == 0) tmp /= d;
        }
        d += (d == 2 ? 1 : 2);
    }
    if (tmp > 1) {
        /* tmp is a prime > sqrt(original T) */
        primes[np++] = tmp;
    }

    /* Find the n-th positive integer coprime to T. */
    /* We binary‑search for the smallest x with count_coprime(x) >= n. */
    idx_t lo = 1, hi = n * 20;  /* initial upper bound */
    if (hi < 1) hi = 1;          /* avoid overflow */
    while (count_coprime(hi, primes, np) < n) hi *= 2;

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (count_coprime(mid, primes, np) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
