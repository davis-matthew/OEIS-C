/*
 * A106813  Smallest prime of the set of six consecutive primes whose sum of
 *          digits is a set of six distinct primes.
 * https://oeis.org/A106813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest prime of a set of six consecutive primes whose *
 *            digit sums are six distinct primes
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   add_seq, is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_seq.h"
#include "../utils/is_prime_uint64.h"

/* Check whether an integer up to 100 is a prime.  Only needed for digit sums. */
static bool isPrimeSmall(uint64_t x)
{
    static const uint64_t primes[] = {
        2,3,5,7,11,13,17,19,23,29,
        31,37,41,43,47,53,59,61,67,71,73,79,83,89,97
    };
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i)
        if (primes[i] == x) return true;
    return false;
}

/* Digit sum of a 64‑bit integer.  Returns a small value (≤ 183). */
static uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t s = 0;
    while (x) {
        s += x % 10;
        x /= 10;
    }
    return s;
}

/* Does the current 6‑prime window satisfy the required property? */
static bool window_ok(const seq_t *win)
{
    uint64_t sums[6];
    for (int i = 0; i < 6; ++i) {
        sums[i] = digit_sum_uint64((uint64_t)win[i]);
        if (!isPrimeSmall(sums[i]))
            return false;
    }
    for (int i = 0; i < 6; ++i)
        for (int j = i + 1; j < 6; ++j)
            if (sums[i] == sums[j])
                return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Static state retained between calls. */
    static seq_t *table      = NULL;
    static idx_t cap = 0, size = 0;
    static uint64_t last_checked = 1;          /* last number tested */
    static seq_t win[6];
    static uint8_t win_cnt = 0;                /* primes in current window */

    if (n == 0) return 0; /* undefined but harmless */

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Find the next prime.  Skip even numbers after 2. */
        uint64_t candidate = last_checked + 1;
        if (candidate > 2 && (candidate & 1ULL) == 0ULL)
            ++candidate;                      /* make odd */
        while (!is_prime_uint64(candidate))
            candidate += 2;                   /* only test odds */
        last_checked = candidate;

        /* Update the sliding window of last six primes. */
        if (win_cnt < 6) {
            win[win_cnt++] = (seq_t)candidate;
            if (win_cnt == 6 && window_ok(win))
                add_seq(&table, &cap, &size, win[0]);       /* record term */
        } else {
            for (int i = 0; i < 5; ++i) win[i] = win[i + 1];
            win[5] = (seq_t)candidate;
            if (window_ok(win))
                add_seq(&table, &cap, &size, win[0]);       /* record term */
        }
    }
    return table[n - 1];
}
