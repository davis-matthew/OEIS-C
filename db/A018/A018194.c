/*
 * A018194  Number of steps for S(S(..S(n)..)) to converge, where S is the
 *          Kempner function A002034.
 * https://oeis.org/A018194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of applications of the Kempner function S * (S is
 *            the least m such that m! is divisible by n) needed to reach a *
 *            fixed point, starting from n. * The iteration stops as soon as
 *            S(x) = x, and the count includes that * last application. For n
 *            <= 1 the result is 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


static uint64_t exponent_in_factorial(uint64_t m, uint64_t p)
{
    uint64_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* Minimal m such that exponent_in_factorial(m,p) >= e.  Binary search
 * is used because exponent_in_factorial(m,p) is non‑decreasing in m. */
static uint64_t min_m_for_prime_exp(uint64_t p, uint64_t e)
{
    uint64_t low = 1;
    uint64_t high = p * e;      /* safe upper bound: in m! the exponent of p
                                  never exceeds m / (p-1) <= m, so m = p*e is enough. */
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (exponent_in_factorial(mid, p) >= e)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Kempner function S(n) for 0 < n <= UINT64_MAX.  For n == 0 or 1 it is
 * defined to be 1. */
static uint64_t kempner_uint64(uint64_t n)
{
    if (n <= 1) return 1;

    uint64_t max_needed = 0;

    /* Trial division factorisation.  Not the fastest but sufficient for
     * n up to the size that is encountered in this sequence (≤ few
     * thousands for the test range). */
    uint64_t temp = n;
    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d != 0) continue;
        uint64_t exp = 0;
        while (temp % d == 0) {
            temp /= d;
            ++exp;
        }
        uint64_t need = min_m_for_prime_exp(d, exp);
        if (need > max_needed) max_needed = need;
    }
    if (temp > 1) {                /* remaining prime factor */
        uint64_t need = min_m_for_prime_exp(temp, 1);
        if (need > max_needed) max_needed = need;
    }

    return max_needed;
}

seq_t a(idx_t n)
{
    seq_t steps = (seq_t)1;       /* count includes the first application of S */
    uint64_t cur = (uint64_t)n;

    while (true) {
        uint64_t nxt = kempner_uint64(cur);
        if (nxt == cur) break;
        ++steps;
        cur = nxt;
    }

    return steps;
}
