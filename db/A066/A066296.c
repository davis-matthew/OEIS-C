/*
 * A066296  a(n) = floor(n^(1/omega(n))).
 * https://oeis.org/A066296
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n^(1/omega(n))) for n >= 2 where omega(n) is the
 *            number of distinct prime factors of n
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 107 terms through n = 980 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/* fast exponentiation by squaring: base^exp in seq_t */
static seq_t seq_pow(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

static seq_t kth_root(seq_t n, unsigned k)
{
    if (k <= 1)
        return n;                 /* n^(1/1) = n */

    seq_t lo = 1;
    seq_t hi = n;
    seq_t best = 1;

    while (lo <= hi) {
        seq_t mid = lo + (hi - lo) / 2;
        seq_t p = seq_pow(mid, (idx_t)k);

        if (p <= n) {
            best = mid;          /* mid^(k) <= n  => mid is a candidate */
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 2)                     /* sequence defined for n >= 2 */
        return 0;

    /* Count distinct prime factors (omega(n)) of the 64‑bit integer n */
    int omega = count_distinct_primes((uint64_t)n);

    /* Safety: omega should be >= 1 for all n >= 2, but guard against zero */
    unsigned k = (omega > 0) ? (unsigned)omega : 1;

    return kth_root((seq_t)n, k);
}
