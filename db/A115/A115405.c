/*
 * A115405  Numbers n such that n^k is deficient for all k>0.
 * https://oeis.org/A115405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 132, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A115405
   Search algorithm: scan positive integers, keep those n for which 2*phi(n) >= n
   (phi = Euler totient).  The result is the n-th such integer in increasing order. */

#include <stdint.h>

/* Cache configuration – sufficient for at least 10000 terms */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static idx_t cache_cnt = 0;      /* how many terms currently stored */
static uint64_t last_checked = 0;/* last integer examined */

/* Append a value to the cache.  The cache is large enough for our needs. */
static void add_entry(uint64_t value)
{
    if (cache_cnt < CACHE_MAX)
        cache[cache_cnt++] = (seq_t)value;
}

/* Compute Euler's totient function by trial division */
static uint64_t compute_phi(uint64_t n)
{
    uint64_t phi = n;
    uint64_t tmp = n;
    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            phi -= phi / p;
        }
    }
    if (tmp > 1)           /* remaining prime factor */
        phi -= phi / tmp;
    return phi;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The offset for the sequence is 1, but behaviour for n == 0 is irrelevant */
    while (cache_cnt < n) {
        ++last_checked;
        uint64_t phi_val = compute_phi(last_checked);
        if (2 * phi_val >= last_checked)
            add_entry(last_checked);
    }
    return cache[n - 1];
}
