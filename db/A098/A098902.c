/*
 * A098902  Even numbers whose number of distinct prime factors is also even.
 * https://oeis.org/A098902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 930 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Even numbers whose number of distinct prime factors is also even.
   The function returns the n‑th such even integer. */
#include <stddef.h>

/* Count the number of distinct prime factors of an idx_t (uint64_t) number. */
static inline uint64_t distinct_prime_cnt_idx(idx_t v)
{
    idx_t t = v;
    uint64_t cnt = 0;

    /* factor 2 */
    if ((t & 1) == 0) {
        cnt++;
        while ((t & 1) == 0) t >>= 1;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if ((t % p) == 0) {
            cnt++;
            while ((t % p) == 0) t /= p;
        }
    }

    if (t > 1) cnt++;  /* remaining prime > sqrt(v) */
    return cnt;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; we look for the n‑th even integer whose
       distinct prime factor count is even. */
    idx_t count = 0;   /* how many qualifying evens we have found */
    idx_t cand   = 2;   /* next even number to test */

    while (count < n) {
        if ((distinct_prime_cnt_idx(cand) & 1ULL) == 0) {
            count++;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 2;  /* next even candidate */
    }

    /* should never reach here */
    return (seq_t)0;
}
