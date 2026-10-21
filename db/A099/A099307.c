/*
 * A099307  Least k such that the k-th arithmetic derivative of n is zero, or
 *          0 if no k exists.
 * https://oeis.org/A099307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that the k-th arithmetic derivative of n is
 *            zero, or 0 if no such k exists. Uses p^p detection as stopping
 *            criterion
 * Range:     u8 n <= 718, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

/* build a prime list once, up to a reasonably large limit */
static void init_primes(void) {
    if (!primes) {
        /* 100'000 is more than enough for values attained when n ≤ 10 000 */
        build_primes_upto(100000, &primes, &primes_cnt);
    }
}

/* factor a uint64_t number into arrays of primes and exponents.
   The caller provides arrays big enough (≤64). */
static void factor_uint64(uint64_t n,
                          uint64_t *pf   /* out: primes */,
                          uint64_t *ex   /* out: exponents */,
                          size_t *cnt    /* out: number of distinct primes */)
{
    size_t c = 0;
    uint64_t temp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pf[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (temp > 1) {          /* remainder is a prime */
        pf[c] = temp;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)n;       /* a(1) = 1; a(0) is unused but safe */

    init_primes();

    seq_t m = (seq_t)n;                /* current value before taking derivative */
    seq_t k = 1;                       /* we are about to compute d^k(m) */

    for (;;) {
        /* factor the current m */
        uint64_t pf[64], ex[64];
        size_t cnt;
        factor_uint64((uint64_t)m, pf, ex, &cnt);

        /* check the p^p stopping condition */
        for (size_t i = 0; i < cnt; ++i) {
            if (ex[i] >= pf[i]) return 0;
        }

        /* compute the arithmetic derivative d(m) = Σ e * (m / p) */
        seq_t d = 0;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t term = (seq_t)ex[i] * (m / (seq_t)pf[i]);
            d += term;
        }

        if (d == 0) return k;          /* we have hit zero after k steps */

        /* prepare for next iteration */
        m = d;
        ++k;
    }
}
