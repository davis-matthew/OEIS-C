/*
 * A095999  Number of divisors of n that are < log(n).
 * https://oeis.org/A095999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d < ln(n) (floor)
 * Range:     u8 n <= 9623, u16 n <= 9623, u32 n <= 9623, u64 n <= 9623,
 *            u128 n <= 9623, big any n
 * Helpers:   count_divs_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <math.h>
#include <stdint.h>
#include "../utils/count_divs_uint64.h"

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
