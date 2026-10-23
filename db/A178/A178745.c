/*
 * A178745  Decimal expansion of Sum_{k>=1} 1/2^Partition(k).
 * https://oeis.org/A178745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   multiply_by_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/******************************************************************************\
 *  OEIS A178745  Decimal expansion of Sum_{k>=1} 1/2^Partition(k)
 *
 *  a(n) = n-th decimal digit (0‑based) of  Σ_{k>=1} 1/2^{p(k)},
 *  where p(k) is the number of partitions of the integer k.
 *
 *  The implementation computes enough terms of the series so that the
 *  remainder is smaller than 10^{-(n+1)}.  The sum is represented as a
 *  rational with denominator 2^max_exp; the numerator is built as a bitset
 *  and ordinary integer arithmetic is used to generate the required decimal
 *  digit.
 *
 *  Complexity: O(max_exp/64 + n) per call, which is below 2 s for n≈10 000.
 *  No global static state other than a small cache of partition numbers.
 *
 *  Requires: C11 / C++17, stdint.h, stdlib.h
 *
 *  Note: The routine is fully deterministic and works for any n>=0.
\******************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>                /* for ceil (used only for one constant) */
#include "../utils/multiply_by_10.h"

static const double LOG2_10 = 3.32192809488736234787;

/* -------------------------------------------------------------------------- */
/*  Partition number cache (up to a given exponent threshold)                 */
/* -------------------------------------------------------------------------- */
static uint32_t   *parts      = NULL;
static size_t      parts_len  = 0;   /* number of stored values (p[0]=1) */
static size_t      parts_cap  = 0;
static idx_t       parts_max_exp = 0;

/* Compute partitions until the exponent grows beyond new_max_exp.
   p[0] is 1 (partition of 0).  Returns the updated parts_len.            */
static void
ensure_parts(idx_t new_max_exp)
{
    if (new_max_exp <= parts_max_exp)
        return;                       /* already done */

    /* initialise if first time */
    if (!parts) {
        parts_cap = 64;
        parts     = (uint32_t*)malloc(parts_cap * sizeof(uint32_t));
        parts[0]  = 1;                /* p(0) */
        parts_len = 1;
    }

    for (size_t k = parts_len; ; ++k) {          /* compute p(k) */
        int64_t sum = 0;
        for (int i = 1; ; ++i) {
            int pent1 = i * (3 * i - 1) / 2;
            if (pent1 > (int)k) break;
            int sign = (i & 1) ? 1 : -1;
            sum += sign * (int64_t)parts[k - pent1];
            int pent2 = i * (3 * i + 1) / 2;
            if (pent2 > (int)k) continue;
            sum += sign * (int64_t)parts[k - pent2];
        }
        uint32_t val = (uint32_t)sum;
        if (val > (uint32_t)new_max_exp) break;   /* done */
        /* store value */
        if (parts_len == parts_cap) {
            parts_cap <<= 1;
            parts = (uint32_t*)realloc(parts, parts_cap * sizeof(uint32_t));
        }
        parts[parts_len++] = val;
    }
    parts_max_exp = new_max_exp;
}

/* -------------------------------------------------------------------------- */
/*  Big integer routine (array of 64‑bit words)                               */

/* -------------------------------------------------------------------------- */
/*  Main sequence function                                                     */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1. determine how many bits of 2^{-exp} are needed for n‑th decimal digit */
    /*    error bound: 2^{-exp} < 10^{-(n+1)}  => exp > (n+1)*log2(10)      */
    /*    Add a safety margin to guarantee this inequality.                   */
    idx_t required_bits = (idx_t)((n + 1) * LOG2_10) + 3;   /* safe upper bound */

    /* 2. Ensure we have computed all partition numbers needed up to that exponent */
    ensure_parts(required_bits);

    /* 3. Build numerator as a bitset (one bit per partition <= required_bits) */
    size_t words = (required_bits + 63) >> 6;        /* words needed for bits */
    size_t words_net = words + 1;                    /* +1 for carries */
    uint64_t *num = (uint64_t*)calloc(words_net, sizeof(uint64_t));

    /* set bits: for each partition p(k) <= required_bits set bit (required_bits - p) */
    for (size_t idx = 1; idx < parts_len; ++idx) {   /* skip p(0) */
        uint32_t exp = parts[idx];
        if (exp > required_bits) break;
        size_t shift = required_bits - exp;
        size_t w     = shift >> 6;
        size_t b     = shift & 63;
        num[w] |= ((uint64_t)1 << b);
    }

    /* 4. Extract the n-th decimal digit by repeated division by 2^required_bits */
    uint64_t digit = 0;
    size_t shift_word = required_bits >> 6;
    unsigned shift_bits = required_bits & 63;

    for (idx_t pos = 0; pos <= n; ++pos) {
        /* multiply current remainder by 10 */
        multiply_by_10(num, words_net);

        /* obtain current digit: floor( num / 2^required_bits ) */
        if (shift_bits == 0) {
            digit = (num[shift_word] & 0xF);
        } else {
            uint64_t low  = num[shift_word] >> shift_bits;
            if (shift_word + 1 < words_net)
                low |= num[shift_word + 1] << (64 - shift_bits);
            digit = low & 0xF;
        }

        /* keep only fractional part: zero all bits >= required_bits */
        if (shift_bits == 0) {
            num[shift_word] = 0;
        } else {
            num[shift_word] &= ((uint64_t)1 << shift_bits) - 1;
        }
        for (size_t j = shift_word + 1; j < words_net; ++j)
            num[j] = 0;
    }

    free(num);
    return (seq_t)digit;
}
