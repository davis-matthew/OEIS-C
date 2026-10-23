/*
 * A045168  Numbers whose base-5 representation contains exactly two 0's and
 *          three 1's.
 * https://oeis.org/A045168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   gcd64, pow3_saturated
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045168: Numbers whose base‑5 representation contains exactly
 * two 0's and three 1's, offset 1.
 *
 * The algorithm enumerates numbers in increasing order by
 * combinatorially counting how many completions exist for the
 * remaining positions.  It handles arbitrary integer sizes via
 * seq_t and idx_t. */

#include <stdint.h>
#include "../utils/gcd64.h"
#include "../utils/pow3_saturated.h"

/* GCD for 64‑bit unsigned integers. */

/* Binomial coefficient C(n,k) with saturation at limit+1.  The
 * function returns min( C(n,k) , limit+1 ). */
static uint64_t binom_saturated(uint64_t n, uint64_t k, uint64_t limit) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    uint64_t result = 1;
    for (uint64_t i = 1; i <= k; ++i) {
        uint64_t a = n - k + i;   /* numerator term   */
        uint64_t b = i;           /* denominator term*/
        uint64_t g = gcd64(a, b);
        a /= g;   b /= g;
        g = gcd64(result, b);
        result /= g;   b /= g;

        if (b != 1) {      /* should not happen for binomial */
            /* fallback: use 128‑bit arithmetic to avoid overflow */
            __uint128_t prod = (__uint128_t)result * a;
            if (b == 0) return limit + 1;
            prod /= b;
            if (prod > limit) return limit + 1;
            result = (uint64_t)prod;
        } else {
            if (result > limit / a) return limit + 1;
            result *= a;
        }
    }
    return result;
}

/* Count completions for remaining positions after a digit is chosen.
 * r   – number of positions left
 * zz  – zeros still needed
 * oo  – ones still needed
 * limit – cap for the result (usually the current offset)   */
static uint64_t count_completions(uint64_t r, uint64_t zz, uint64_t oo,
                                  uint64_t limit) {
    if (zz > r || oo > r || zz + oo > r) return 0;
    uint64_t left = r - zz - oo;

    uint64_t c1 = binom_saturated(r, zz, limit);
    if (c1 > limit) return limit + 1;
    uint64_t c2 = binom_saturated(r - zz, oo, limit);
    if (c2 > limit) return limit + 1;
    uint64_t p3 = pow3_saturated(left, limit);
    if (p3 > limit) return limit + 1;

    if (c1 > limit / c2) return limit + 1;
    uint64_t t = c1 * c2;
    if (t > limit / p3) return limit + 1;
    t *= p3;
    return t;
}

/* Count how many length‑L numbers satisfy the conditions, capped at
 * limit+1.  The limit is the remaining position index within the
 * current block. */
static uint64_t count_len(uint64_t L, uint64_t limit) {
    if (L < 5) return 0;
    /* first digit = 1 */
    uint64_t c1 = count_completions(L - 1, 2, 2, limit);
    /* first digit = 2,3,4 (three choices) */
    uint64_t c2 = count_completions(L - 1, 2, 3, limit);
    uint64_t mulc2;
    if (c2 > limit / 3) mulc2 = limit + 1;
    else mulc2 = c2 * 3;
    uint64_t total;
    if (c1 > limit - mulc2) total = limit + 1;
    else total = c1 + mulc2;
    return total;
}

/* Main sequence function.  idx_t is unsigned 64‑bit; seq_t is the
 * required output integer type. */
seq_t a(idx_t n) {
    /* n ≥ 1 is guaranteed by the problem statement. */
    uint64_t remaining = (uint64_t)n;   /* index within the current block */
    uint64_t L = 5;                     /* minimum length */

    /* Determine the length L that contains the n‑th term. */
    while (1) {
        uint64_t cnt = count_len(L, remaining);
        if (remaining > cnt) {
            remaining -= cnt;
            ++L;
        } else {
            break;
        }
    }

    /* Construct the number digit by digit. */
    uint64_t zeros_needed = 2;
    uint64_t ones_needed  = 3;
    seq_t result = 0;

    for (uint64_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d < 5; ++d) {
            if (pos == 0 && d == 0) continue;   /* no leading zero */

            uint64_t zz = zeros_needed;
            uint64_t oo = ones_needed;

            if (d == 0) {
                if (zz == 0) continue;
                --zz;
            } else if (d == 1) {
                if (oo == 0) continue;
                --oo;
            }

            uint64_t r = L - pos - 1;            /* remaining positions */
            if (zz > r || oo > r) continue;

            uint64_t completions = count_completions(r, zz, oo, remaining);
            if (completions < remaining) {
                remaining -= completions;
                continue;   /* skip this digit */
            }

            /* Choose this digit and continue. */
            result = result * (seq_t)5 + (seq_t)d;
            zeros_needed = zz;
            ones_needed  = oo;
            break;      /* digit fixed for this position */
        }
    }

    return result;
}
