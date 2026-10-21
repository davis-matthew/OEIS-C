/*
 * A083895  Number of divisors of n with largest digit = 8 (base 10).
 * https://oeis.org/A083895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of positive divisors d of n whose largest decimal
 *            digit is 8
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* largest decimal digit of a 64‑bit integer */
static inline int largest_digit_uint64(uint64_t x)
{
    int max = 0;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d > max) max = d;
        x /= 10;
    }
    return max;
}

/* recursive generation of divisors and counting those with largest digit 8 */
static void count_divisors(const uint64_t *pr, const uint64_t *exp,
                           size_t idx, size_t cnt, uint64_t cur,
                           seq_t *res)
{
    if (idx == cnt) {
        if (largest_digit_uint64(cur) == 8) (*res)++;
        return;
    }
    uint64_t p = pr[idx];
    uint64_t cur_mul = 1;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        count_divisors(pr, exp, idx + 1, cnt, cur * cur_mul, res);
        cur_mul *= p;
    }
}

/* exported function matching OEIS indexing */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* factor n by trial division */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    uint64_t rem = (uint64_t)n;

    /* factor 2 */
    if (rem % 2 == 0) {
        uint64_t e = 0;
        while (rem % 2 == 0) {
            rem /= 2;
            e++;
        }
        primes[pf_cnt] = 2;
        exps[pf_cnt] = e;
        pf_cnt++;
    }

    /* odd factors */
    uint64_t p = 3;
    while (p * p <= rem) {
        if (rem % p == 0) {
            uint64_t e = 0;
            while (rem % p == 0) {
                rem /= p;
                e++;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt] = e;
            pf_cnt++;
        }
        p += 2;
    }
    if (rem > 1) {
        primes[pf_cnt] = rem;
        exps[pf_cnt] = 1;
        pf_cnt++;
    }

    seq_t count = 0;
    count_divisors(primes, exps, 0, pf_cnt, 1, &count);
    return count;
}
