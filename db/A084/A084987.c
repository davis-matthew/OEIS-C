/*
 * A084987  Duplicate of A083185.
 * https://oeis.org/A084987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th prime palindrome whose decimal digits are all in
 *            {0,1,4,6,8,9}
 * Range:     i8 n <= 2, i16 n <= 13, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}
#include <stddef.h>
#include <stdbool.h>

/* allowed digits */
static const char first_digits[2] = {'1','9'};
static const char all_digits[6]   = {'0','1','4','6','8','9'};
static const int num_first_digits = 2;
static const int num_all_digits   = 6;

/* recursive generators */
static void gen_even_rec(int pos, int halfLen, int totalLen,
                         char *buf, idx_t target,
                         idx_t *cnt, seq_t *result, bool *found)
{
    if (*found) return;
    if (pos == halfLen) {
        /* mirror left half to right half */
        for (int i = 0; i < halfLen; ++i)
            buf[halfLen + i] = buf[halfLen - 1 - i];
        buf[totalLen] = '\0';
        seq_t val = seq_from_str(buf);
        if (is_prime_big(val)) {
            (*cnt)++;
            if (*cnt == target) {
                *result = val;
                *found = true;
            }
        }
        return;
    }
    const char *digits = (pos == 0) ? first_digits : all_digits;
    int nd = (pos == 0) ? num_first_digits : num_all_digits;
    for (int d = 0; d < nd; ++d) {
        buf[pos] = digits[d];
        gen_even_rec(pos + 1, halfLen, totalLen, buf, target, cnt, result, found);
        if (*found) return;
    }
}

static void gen_odd_rec(int pos, int leftLen, int totalLen,
                        char *buf, idx_t target,
                        idx_t *cnt, seq_t *result, bool *found)
{
    if (*found) return;
    if (pos == leftLen) {
        for (int mi = 0; mi < num_all_digits; ++mi) {
            buf[leftLen] = all_digits[mi];
            for (int i = 0; i < leftLen; ++i)
                buf[leftLen + 1 + i] = buf[leftLen - 1 - i];
            buf[totalLen] = '\0';
            seq_t val = seq_from_str(buf);
            if (is_prime_big(val)) {
                (*cnt)++;
                if (*cnt == target) {
                    *result = val;
                    *found = true;
                    return;
                }
            }
        }
        return;
    }
    const char *digits = (pos == 0) ? first_digits : all_digits;
    int nd = (pos == 0) ? num_first_digits : num_all_digits;
    for (int d = 0; d < nd; ++d) {
        buf[pos] = digits[d];
        gen_odd_rec(pos + 1, leftLen, totalLen, buf, target, cnt, result, found);
        if (*found) return;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t answer = 0;
    bool found = false;

    for (int length = 2; !found; ++length) {
        if (length % 2 == 0) {
            int halfLen  = length / 2;
            int totalLen = length;
            char buf[64];
            gen_even_rec(0, halfLen, totalLen, buf, n, &count, &answer, &found);
        } else {
            int leftLen  = (length - 1) / 2;
            int totalLen = length;
            char buf[64];
            gen_odd_rec(0, leftLen, totalLen, buf, n, &count, &answer, &found);
        }
    }
    return answer;
}
