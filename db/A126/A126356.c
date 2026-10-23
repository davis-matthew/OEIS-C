/*
 * A126356  a(n) is the n-th integer from among the positive integers which
 *          are coprime to (n+1).
 * https://oeis.org/A126356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that is coprime to (n+1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Count how many integers ≤ x are coprime to m.
   The distinct prime factors of m are in pr[0..pcnt-1]. */
static idx_t count_coprime(idx_t x, const uint64_t *pr, size_t pcnt)
{
    idx_t cnt = x;
    /* Inclusion‑exclusion over the distinct primes of m. */
    for (size_t mask = 1; mask < ((size_t)1 << pcnt); ++mask) {
        uint64_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & ((size_t)1 << i)) {
                prod *= pr[i];
                ++bits;
            }
        }
        idx_t term = x / (idx_t)prod;
        if (bits & 1U)
            cnt -= term;
        else
            cnt += term;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* m = n + 1; collect its distinct prime factors. */
    uint64_t m = (uint64_t)n + 1;
    uint64_t primes[64];
    size_t pcnt = 0;
    uint64_t tmp = m;

    if (tmp % 2 == 0) {
        primes[pcnt++] = 2;
        while (tmp % 2 == 0)
            tmp /= 2;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            primes[pcnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pcnt++] = tmp;

    /* Binary search for the smallest x with at least n coprime numbers ≤ x. */
    idx_t lo = 1;
    idx_t hi = n * (n + 1);          /* safe upper bound for all n of interest. */

    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t cnt = count_coprime(mid, primes, pcnt);
        if (cnt >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
