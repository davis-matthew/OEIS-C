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
 * Helpers:   is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"
#include "../utils/seq_from_str.h"
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
