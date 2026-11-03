/*
 * A193688  Number of steps to reach 1 in Collatz (3x+1) problem starting with
 *          2^n - 1.
 * https://oeis.org/A193688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of steps to reach 1 in Collatz starting from 2^n − 1
 * Range:     u8 n <= 20, u16 n <= 4848, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9102 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void big_add1(uint64_t *a, size_t *words) {
    uint64_t carry = 1;
    for (size_t i = 0; i < *words; ++i) {
        uint64_t old = a[i];
        a[i] += carry;
        if (a[i] < old)   /* overflow */
            carry = 1;
        else
            carry = 0;
        if (!carry) break;
    }
    if (carry) {
        a[*words] = 1;
        ++(*words);
    }
}

static void big_init_from_2pow_minus1(uint64_t *a, size_t *words,
                                     idx_t n) {
    *words = (n + 63) / 64;
    for (size_t i = 0; i < *words; ++i) a[i] = ~0ULL;
    if ((n & 63) == 0) {
        /* last word is already all ones */
    } else {
        uint64_t mask = (1ULL << (n & 63)) - 1;
        a[*words - 1] = mask;
    }
}

static int big_is_one(const uint64_t *a, size_t words) {
    return (words == 1 && a[0] == 1);
}

/* small helpers for big integer arithmetic */
static void big_mult3(uint64_t *a, size_t *words) {
    uint64_t carry = 0;
    for (size_t i = 0; i < *words; ++i) {
        unsigned __int128 prod = (unsigned __int128)a[i] * 3 + carry;
        a[i] = (uint64_t)prod;
        carry  = (uint64_t)(prod >> 64);
    }
    if (carry) {
        a[*words] = carry;
        ++(*words);
    }
}

static void big_shift_right(uint64_t *a, size_t *words,
                           unsigned shift) {
    if (shift == 0) return;
    unsigned full = shift / 64;
    unsigned rem  = shift % 64;

    size_t w = *words;
    if (full) {
        if (full >= w) {
            /* shift away entire number: becomes zero */
            w = 1;
            a[0] = 0;
            *words = w;
            return;
        }
        for (size_t i = 0; i < w - full; ++i) {
            a[i] = a[i + full];
        }
        w -= full;
    }

    if (rem) {
        uint64_t carry = 0;
        for (size_t i = w; i > 0; --i) {
            size_t idx = i - 1;
            uint64_t newcarry = a[idx] << (64 - rem);
            a[idx] = (a[idx] >> rem) | carry;
            carry = newcarry;
        }
    }

    while (w > 1 && a[w - 1] == 0) --w;
    *words = w;
}

static unsigned big_trailing_zeros(const uint64_t *a, size_t words) {
    for (size_t i = 0; i < words; ++i) {
        if (a[i]) {
            return (unsigned)(i * 64 + __builtin_ctzll(a[i]));
        }
    }
    return (unsigned)(words * 64);  /* should not happen for non‑zero number */
}

#define MAXWORDS 4000  /* enough for n up to ~256 000 bits */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1, but safe */

    /* Starting with 2^n - 1 */
    static uint64_t big[MAXWORDS];
    size_t words;
    big_init_from_2pow_minus1(big, &words, n);

    seq_t steps = 0;
    while (!big_is_one(big, words)) {
        unsigned tz = big_trailing_zeros(big, words);
        if (tz) {                     /* even number */
            big_shift_right(big, &words, tz);
            steps += tz;
            continue;
        }
        /* odd number: 3x+1 step */
        big_mult3(big, &words);
        big_add1(big, &words);
        steps += 1;                   /* count the 3x+1 step */
        unsigned tz2 = big_trailing_zeros(big, words);
        if (tz2) {
            big_shift_right(big, &words, tz2);
            steps += tz2;
        }
    }
    return steps;
}
