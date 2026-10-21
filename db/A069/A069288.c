/*
 * A069288  Number of odd divisors of n <= sqrt(n).
 * https://oeis.org/A069288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | d divides n, d odd, d ≤ √n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_u64, _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the floor integer square root of an idx_t value. */
#include "../utils/_floor_isqrt_u64.h"
/* Factor a uint64_t into prime factors and exponents. */
#include "../utils/_factor_u64.h"

#include <stddef.h>
#include <stdint.h>

/* Count divisors of the odd part of n that are ≤ limit. */
static void count_divs(idx_t idx, idx_t cur, const uint64_t *pr,
                       const uint64_t *exp, size_t cnt, idx_t limit,
                       seq_t *total)
{
    if (idx == cnt) {
        /* cur is a divisor of the odd part – it is already ≤ limit. */
        (*total)++;
        return;
    }
    uint64_t p   = pr[idx];
    uint64_t expv = exp[idx];
    idx_t prod = cur;

    for (uint64_t e = 0; e <= expv; ++e) {
        if (prod > limit) break;          /* too large – stop this branch */
        count_divs(idx + 1, prod, pr, exp, cnt, limit, total);
        if (e == expv) break;
        /* Prepare for next exponent: multiply by p if still within limit. */
        if (prod > limit / p) break;       /* would exceed limit or overflow */
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    /* Remove all even factors; odd divisors come from the odd part. */
    idx_t odd = n;
    while ((odd & 1) == 0) odd >>= 1;

    /* Limit for divisors: floor(sqrt(n)). */
    idx_t limit = _floor_isqrt_u64(n);

    /* Factor the odd part.  Using temporary arrays of sufficient size. */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;
    _factor_u64(odd, pr, exp, &cnt);

    /* Enumerate divisors ≤ limit and count them. */
    seq_t result = 0;
    count_divs(0, 1, pr, exp, cnt, limit, &result);

    return result;
}
