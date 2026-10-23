/*
 * A132273  a(n) = Sum{k=1..n} (k-th integer from among those positive
 *          integers that are coprime to (n+1-k)).
 * https://oeis.org/A132273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} kth_coprime(n+1-k, k)
 * Range:     u8 n <= 16, u16 n <= 261, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------- helper: factorization of a uint64_t number into distinct primes ------------- */
#include <stddef.h>
#include <stdint.h>

/* factor_uint64 returns the number of distinct prime factors of n and fills
   array pr (which must be large enough).  The input n is assumed to fit in
   uint64_t (which is true for m = n+1-i). */
static size_t factor_uint64(uint64_t n, uint32_t *pr)
{
    size_t cnt = 0;
    if (n % 2ULL == 0ULL) {
        pr[cnt++] = 2U;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    }
    for (uint64_t p = 3ULL; p * p <= n; p += 2ULL) {
        if (n % p == 0ULL) {
            pr[cnt++] = (uint32_t)p;
            while (n % p == 0ULL) n /= p;
        }
    }
    if (n > 1ULL) {
        pr[cnt++] = (uint32_t)n;
    }
    return cnt;
}

/* count_coprime counts how many integers ≤ x are coprime to all primes in
   the array pr[0..cnt-1].  Inclusion–exclusion is used. */
static seq_t count_coprime(seq_t x, const uint32_t *pr, size_t cnt)
{
    if (x == (seq_t)0)
        return (seq_t)0;
    seq_t res = x;
    for (size_t mask = 1; mask < (1ULL << cnt); ++mask) {
        seq_t prod = (seq_t)1;
        int bits = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (mask & (1ULL << i)) {
                prod *= (seq_t)pr[i];
                ++bits;
            }
        }
        seq_t term = x / prod;
        if (bits & 1)
            res -= term;
        else
            res += term;
    }
    return res;
}

/* kth_coprime returns the k-th positive integer that is coprime to m
   (m ≥ 1, k ≥ 1). */
static seq_t kth_coprime(seq_t m, idx_t k)
{
    if (m == (seq_t)1)           /* every positive integer is coprime to 1 */
        return (seq_t)k;

    uint32_t primes[16];
    size_t kcnt = factor_uint64((uint64_t)m, primes);

    seq_t low  = (seq_t)1;
    seq_t high = (seq_t)k * m;   /* initial upper bound, safe for n ≤ 10000 */

    /* Ensure high is large enough */
    while (count_coprime(high, primes, kcnt) < (seq_t)k) {
        high = high + high;      /* double high */
    }

    /* Binary search for smallest x with count(x) ≥ k */
    while (low < high) {
        seq_t mid = low + ((high - low) >> 1);
        if (count_coprime(mid, primes, kcnt) >= (seq_t)k)
            high = mid;
        else
            low = mid + (seq_t)1;
    }
    return low;
}

/* ---------------- main sequence function ---------------- */
seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t i = (idx_t)1; i <= n; ++i) {
        idx_t m = n + 1 - i;   /* coprime target */
        idx_t k = i;           /* k-th element in that list */
        sum += kth_coprime((seq_t)m, k);
    }
    return sum;
}
