/*
 * A073029  Names for numbers in American English, with each letter
 *          transformed into its index in the alphabet.
 * https://oeis.org/A073029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  word, nonn, easy, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 9250, u16 n <= 9250, u32 n <= 9250, u64 n <= 9250,
 *            u128 n <= 9250, big any n
 * Helpers:   append_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by generating the English name for the nth number in
   the triangle of alphabet indices. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/append_str.h"

static const char *DIGITS[10] = {"zero","one","two","three","four","five","six","seven","eight","nine"};
static const char *TEENS[10] = {"ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
static const char *TENS[10]   = {"","", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};
static const char *THOUSANDS[12] = {"", "thousand","million","billion","trillion","quadrillion","quintillion",
                                    "sextillion","septillion","octillion","nonillion","decillion"};

static void append_group(int g, int level, char *buf, size_t *pos)
{
    if (g == 0) return;

    if (g >= 100) {
        int hundreds = g / 100;
        append_str(DIGITS[hundreds], buf, pos);
        append_str("hundred", buf, pos);
    }

    int r = g % 100;
    if (r >= 20) {
        int t = r / 10;
        append_str(TENS[t], buf, pos);
        int o = r % 10;
        if (o) append_str(DIGITS[o], buf, pos);
    } else if (r >= 10) {
        append_str(TEENS[r - 10], buf, pos);
    } else if (r) {
        append_str(DIGITS[r], buf, pos);
    }

    if (level > 0 && level < (int)(sizeof(THOUSANDS) / sizeof(THOUSANDS[0]))) {
        append_str(THOUSANDS[level], buf, pos);
    }
}

static void number_to_letters(uint64_t n, char *buf, size_t *len)
{
    if (n == 0) {
        *len = 0;
        append_str(DIGITS[0], buf, len);
        return;
    }

    int groups[16];
    size_t cnt = 0;
    uint64_t temp = n;
    while (temp && cnt < sizeof(groups)/sizeof(groups[0])) {
        groups[cnt++] = temp % 1000;
        temp /= 1000;
    }

    size_t pos = 0;
    for (int i = (int)cnt - 1; i >= 0; --i) {
        int level = i;      /* 0 = units, 1 = thousand, etc. */
        int g = groups[i];
        append_group(g, level, buf, &pos);
    }
    *len = pos;
}

seq_t a(idx_t n)
{
    uint64_t target = n;
    uint64_t number = 0;

    while (1) {
        char buf[512];
        size_t len;
        number_to_letters(number, buf, &len);

        if (target < (uint64_t)len) {
            char ch = buf[target];
            return (seq_t)(ch - 'a' + 1);  /* 'a'->1 ... 'z'->26 */
        }
        target -= len;
        number++;
    }
}
