/*
 * A192787  Number of distinct solutions of 4/n = 1/a + 1/b + 1/c in positive
 *          integers satisfying 1 <= a <= b <= c.
 * https://oeis.org/A192787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of distinct solutions of 4/n = 1/a + 1/b + 1/c in
 *            positive integers satisfying 1 ≤ a ≤ b ≤ c
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Euclidean algorithm for 64‑bit integers. */
static uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Recursively generate all divisors d of q² with d ≤ q.
   Count those for which (d + q) is divisible by p and produce
   a valid b = (d + q)/p that satisfies b ≥ a. */
static void count_divisors_rec(size_t idx, uint64_t cur,
                               const uint64_t *fac, const int *exp,
                               size_t pc,
                               uint64_t q, uint64_t p, uint64_t a,
                               uint64_t *cnt)
{
    if (idx == pc) {
        if (cur <= q && (cur + q) % p == 0) {
            uint64_t b = (cur + q) / p;
            if (b >= a) {
                ++(*cnt);
            }
        }
        return;
    }

    uint64_t mult = 1;
    int maxe = exp[idx] * 2;              /* exponent in q² */
    for (int e = 0; e <= maxe; ++e) {
        if (cur > q / mult) break;        /* cur * mult would exceed q */
        count_divisors_rec(idx + 1, cur * mult, fac, exp, pc,
                           q, p, a, cnt);
        if (e == maxe) break;
        if (mult > q / fac[idx]) break;   /* avoid overflow of the multiplier */
        mult *= fac[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t total = 0;

    /* a must satisfy n/4 < a ≤ 3n/4 */
    idx_t lower = n / 4 + 1;
    idx_t upper = (3 * n) / 4;

    for (idx_t ai = lower; ai <= upper; ++ai) {
        uint64_t a_val = (uint64_t)ai;
        uint64_t r = 4 * a_val - (uint64_t)n;    /* 4a – n > 0 */
        uint64_t d = a_val * (uint64_t)n;

        uint64_t g = gcd_u64(d, r);
        uint64_t p = r / g;                     /* reduced numerator   */
        uint64_t q = d / g;                     /* reduced denominator */

        /* Factorise q */
        uint64_t fac[20];
        int exp[20];
        size_t pc = 0;
        uint64_t t = q;

        if (t % 2 == 0) {
            int e = 0;
            while ((t & 1ull) == 0) {
                t >>= 1;
                ++e;
            }
            fac[pc] = 2ull;
            exp[pc] = e;
            ++pc;
        }

        for (uint64_t f = 3; f * f <= t; f += 2) {
            if (t % f == 0) {
                int e = 0;
                while (t % f == 0) {
                    t /= f;
                    ++e;
                }
                fac[pc] = f;
                exp[pc] = e;
                ++pc;
            }
        }

        if (t > 1) {
            fac[pc] = t;
            exp[pc] = 1;
            ++pc;
        }

        uint64_t cnt = 0;
        count_divisors_rec(0, 1, fac, exp, pc,
                           q, p, a_val, &cnt);

        total += (seq_t)cnt;
    }

    return total;
}
