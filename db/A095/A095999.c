/*
 * A095999  Number of divisors of n that are < log(n).
 * https://oeis.org/A095999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d < ln(n) (floor)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9623 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <math.h>
#include <stdint.h>

/* Helper: recursively count divisors <= threshold.
   cur   – current partial divisor value (uint64_t).
   idx   – current index in the factor arrays.
   pc    – total number of distinct prime factors.
   pf    – array of prime factors.
   exp   – array of exponents.
   th    – threshold value (uint64_t).
   cnt   – pointer to the running count (seq_t). */
static void count_divs_uint64(uint64_t cur, size_t idx, size_t pc,
                              const uint64_t *pf, const int *exp,
                              uint64_t th, seq_t *cnt)
{
    if (idx == pc) {
        if (cur <= th) (*cnt)++;
        return;
    }
    uint64_t p = pf[idx];
    int e = exp[idx];
    uint64_t val = cur;
    for (int k = 0; k <= e; ++k) {
        if (val <= th)
            count_divs_uint64(val, idx + 1, pc, pf, exp, th, cnt);
        if (k == e) break;
        if (val > th / p) break;  /* next multiplication would exceed threshold */
        val *= p;
    }
}

/* Helper: factor n into prime powers.
   pf   – array to store prime factors (uint64_t).
   exp  – array to store corresponding exponents (int).
   pc   – pointer to size_t storing number of distinct prime factors.
   n    – the integer to factor (uint64_t). */
static void factor_uint64(uint64_t n, uint64_t *pf, int *exp, size_t *pc)
{
    size_t count = 0;
    for (uint64_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            int e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            pf[count] = d;
            exp[count] = e;
            ++count;
        }
    }
    if (n > 1) {          /* remaining prime factor */
        pf[count] = n;
        exp[count] = 1;
        ++count;
    }
    *pc = count;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* threshold = floor( ln(n) ).  Double is used only for this small integer.
       It is safe because the error is far below 0.5 for all n that fit in uint64_t. */
    double logd = log((double)(uint64_t)n);
    uint64_t threshold = (uint64_t)(logd + 1e-12);   /* subtract tiny epsilon */

    /* Factor n.  We use up to 64 different primes, which is safe for uint64_t. */
    uint64_t pf[64];
    int exp[64];
    size_t pc;
    factor_uint64((uint64_t)n, pf, exp, &pc);

    /* Count divisors <= threshold. */
    seq_t cnt = 0;
    count_divs_uint64(1, 0, pc, pf, exp, threshold, &cnt);

    return cnt;
}
