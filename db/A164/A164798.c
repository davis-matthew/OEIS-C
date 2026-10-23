/*
 * A164798  a(n) = the smallest integer >= n such that a(n)!/(n-1)! is
 *          divisible by every prime from 2 to the largest prime divisor of
 *          a(n)!/(n-1)!. (a(1)=1.)
 * https://oeis.org/A164798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer >= n such that a(n)!/(n-1)! is
 *            divisible by every prime from 2 to the largest prime divisor of
 *            a(n)!/(n-1)!
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAXP 500000   /* upper bound of primes needed for n up to ~10000 */

/* global data for the prime sieve */
static uint32_t spf[MAXP + 1];        /* smallest prime factor */
static int prime_idx[MAXP + 1];       /* index of a prime or -1 */
static uint32_t primes_arr[MAXP];     /* primes list, size large enough */
static unsigned int primes_cnt = 0;   /* number of primes */
static unsigned char seen[MAXP];     /* temporary seen array, used per call */

/* build linear sieve */
static void init_sieve(void) {
    size_t i, j;
    for (i = 0; i <= MAXP; ++i) {
        spf[i] = 0;
        prime_idx[i] = -1;
    }
    for (i = 2; i <= MAXP; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            prime_idx[i] = (int)primes_cnt;
            primes_arr[primes_cnt++] = (uint32_t)i;
        }
        for (j = 0; j < primes_cnt && primes_arr[j] <= spf[i] &&
             i * (uint64_t)primes_arr[j] <= MAXP; ++j) {
            spf[i * primes_arr[j]] = primes_arr[j];
        }
    }
}

/* compute a(n) */
seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        init_sieve();
        initialized = true;
    }
    if (n == 1) return (seq_t)1;   /* trivial case */

    uint32_t m = (uint32_t)n;
    memset(seen, 0, primes_cnt);   /* reset seen array */

    unsigned int seenCnt = 0;      /* number of distinct primes seen */
    int seenMaxIdx = -1;           /* index of largest prime seen */

    while (1) {
        uint32_t x = m;
        while (x > 1) {
            uint32_t p = spf[x];
            int idx = prime_idx[p];
            if (!seen[idx]) {
                seen[idx] = 1;
                ++seenCnt;
                if (idx > seenMaxIdx) seenMaxIdx = idx;
            }
            while (x % p == 0) x /= p;
        }
        /* check the condition: all primes up to seenMaxIdx are present */
        if (seenCnt == (unsigned int)(seenMaxIdx + 1)) {
            return (seq_t)m;
        }
        ++m;
        /* safety guard – m should never exceed MAXP with the given bounds */
        if (m > MAXP) {
            return (seq_t)m;
        }
    }
}
