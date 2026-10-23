/*
 * A164794  a(n) is the smallest number whose English name has the letter "f"
 *          in the n-th position, or -1 if no such number exists.
 * https://oeis.org/A164794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, sign
 * Formula:   a(n) = smallest integer whose English name has an 'f' in its
 *            n-th position (1‑based). Return -1 if no such integer exists
 * Range:     i8 n <= 11, i16 n <= 39, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: GNU C11 / C++17 compatible. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

static const char *ONES[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *TENS[] = {
    "",     "",      "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};

static void append(const char *s, char *buf, size_t *p)
{
    size_t l = strlen(s);
    memcpy(buf + *p, s, l);
    *p += l;
}

/* Write numbers 1..999 */
static void write_chunk(uint64_t n, char *buf, size_t *p)
{
    if (n >= 100) {
        uint64_t h = n / 100;
        append(ONES[h], buf, p);
        append("hundred", buf, p);
        n %= 100;
        if (n == 0) return;
    }
    if (n < 20) {
        append(ONES[n], buf, p);
        return;
    }
    uint64_t t = n / 10;
    append(TENS[t], buf, p);
    n %= 10;
    if (n > 0) {
        append(ONES[n], buf, p);
    }
}

static void number_to_words(uint64_t n, char *buf, size_t *len)
{
    *len = 0;
    if (n == 0) {
        append("zero", buf, len);
        return;
    }
    if (n >= 1000000000ULL) { /* billions */
        uint64_t part = n / 1000000000ULL;
        write_chunk(part, buf, len);
        append("billion", buf, len);
        n %= 1000000000ULL;
    }
    if (n >= 1000000ULL) { /* millions */
        uint64_t part = n / 1000000ULL;
        write_chunk(part, buf, len);
        append("million", buf, len);
        n %= 1000000ULL;
    }
    if (n >= 1000ULL) { /* thousands */
        uint64_t part = n / 1000ULL;
        write_chunk(part, buf, len);
        append("thousand", buf, len);
        n %= 1000ULL;
    }
    if (n > 0) {
        write_chunk(n, buf, len);
    }
}

/* Main function required by the challenge */
seq_t a(idx_t n)
{
    /* small buffer is sufficient for numbers up to billions */
    char buf[256];
    size_t len;
    /* arbitrarily large search limit, enough for n up to 10000 */
    const uint64_t LIMIT = 2000000ULL;
    for (uint64_t num = 1; num <= LIMIT; ++num) {
        number_to_words(num, buf, &len);
        if (n <= len) {
            char c = buf[n-1];
            if (c == 'f' || c == 'F') {
                return (seq_t)num;
            }
        }
    }
    return (seq_t)-1;
}
