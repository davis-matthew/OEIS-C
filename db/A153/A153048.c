/*
 * A153048  Numbers n with property that n^2 is a concatenation of two 3-digit
 *          primes.
 * https://oeis.org/A153048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   is_three_digit_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search n from 317 to 999 and keep those n such that
   n^2 is the concatenation of two 3‑digit primes. */
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_three_digit_prime.h"

#define MEMO_CAP 16  /* at most 15 numbers known */

/* Cache of found numbers. */
static seq_t memo[MEMO_CAP];
static idx_t memo_count = 0;
static bool memo_built = false;

/* Build the cache lazily. */
static void build_memo(void)
{
    unsigned int start = 317;      /* sqrt(100100) ≈ 317 */
    unsigned int end   = 999;      /* sqrt(999999) ≈ 999 */

    size_t cnt = 0;
    for (unsigned int n = start; n <= end; ++n) {
        seq_t sq = (seq_t)n * (seq_t)n;
        seq_t hi = sq / 1000;     /* leading three digits */
        seq_t lo = sq % 1000;     /* trailing three digits */
        if (is_three_digit_prime((unsigned int)hi) &&
            is_three_digit_prime((unsigned int)lo)) {
            if (cnt < MEMO_CAP)
                memo[cnt++] = (seq_t)n;
        }
    }
    memo_count = (idx_t)cnt;
    memo_built = true;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* no a(0) defined */
    if (!memo_built)
        build_memo();
    if (n <= memo_count)
        return memo[n - 1];
    return (seq_t)0;              /* undefined beyond end of list */
}
