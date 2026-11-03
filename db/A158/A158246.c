/*
 * A158246  Positive numbers that have at least one two-letter substring
 *          repeated when spelled out in English.
 * https://oeis.org/A158246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search.
   a(n) = the nth positive integer whose English name (with spaces and hyphens removed)
   contains at least one 2‑letter substring occurring twice.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Return true if the zero‑terminated string s contains any 2‑letter
   substring that appears at least twice. */
static bool has_repeated_two_letters(const char *s)
{
    size_t len = strlen(s);
    if (len < 2) return false;

    bool seen[26 * 26];
    memset(seen, 0, sizeof(seen));

    for (size_t i = 0; i + 1 < len; ++i) {
        int a = s[i] - 'a';
        int b = s[i + 1] - 'a';
        int idx = a * 26 + b;
        if (seen[idx]) return true;
        seen[idx] = true;
    }
    return false;
}

/* English words for 0–19 and tens multiples. */
static const char *const small[] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};
static const char *const tens[] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy",
    "eighty","ninety"
};

/* Append zero‑terminated string *src to *dst and advance *dst. */
static void append(char **dst, const char *src)
{
    while (*src) *(*dst)++ = *src++;
}

/* Recursive routine converting a number n (n > 0) into its English
   representation written in lowercase, with no spaces or hyphens. */
static void to_words(uint64_t n, char **dst)
{
    if (n >= 1000000000ULL) {
        to_words(n / 1000000000ULL, dst);
        append(dst, "billion");
        n %= 1000000000ULL;
        if (n == 0) return;
    }
    if (n >= 1000000ULL) {
        to_words(n / 1000000ULL, dst);
        append(dst, "million");
        n %= 1000000ULL;
        if (n == 0) return;
    }
    if (n >= 1000ULL) {
        to_words(n / 1000ULL, dst);
        append(dst, "thousand");
        n %= 1000ULL;
        if (n == 0) return;
    }
    if (n >= 100ULL) {
        to_words(n / 100ULL, dst);
        append(dst, "hundred");
        n %= 100ULL;
        if (n == 0) return;
    }
    if (n >= 20ULL) {
        append(dst, tens[n / 10ULL]);
        n %= 10ULL;
        if (n == 0) return;
    }
    if (n > 0) {
        append(dst, small[n]);
    }
}

/* Compute the nth term of A158246. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t num = 1;

    while (1) {
        char buf[128];
        char *p = buf;
        to_words(num, &p);
        *p = '\0';           /* terminate */

        if (has_repeated_two_letters(buf)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
        ++num;
    }
}
