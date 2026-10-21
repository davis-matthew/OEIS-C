/*
 * A092302  Delete all letters except c,d,i,l,m,u,v,x from n then read as
 *          Roman numeral if possible, otherwise 0.
 * https://oeis.org/A092302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = 0 if the string formed by keeping only the letters
 *            c,d,i,l,m,u,v,x (with U treated as V) in the English word of n
 *            is not a valid Roman numeral; otherwise it is the Roman numeral
 *            value (U counts as V)
 * Range:     u8 n <= 34, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   add_str, roman_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/add_str.h"
#include "../utils/roman_value.h"

static const char *const units_word[] =
    {"","one","two","three","four","five","six","seven","eight","nine"};
static const char *const teens_word[] =
    {"ten","eleven","twelve","thirteen","fourteen","fifteen",
     "sixteen","seventeen","eighteen","nineteen"};
static const char *const tens_word[] =
    {"","", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

static void add_group(idx_t n, char *p, size_t *len)
{
    if (n >= 100) {
        add_str(p, units_word[n / 100], len);
        add_str(p, "hundred", len);
        n %= 100;
    }
    if (n >= 20) {
        add_str(p, tens_word[n / 10], len);
        n %= 10;
    }
    if (n >= 10) {
        add_str(p, teens_word[n - 10], len);
    } else if (n > 0) {
        add_str(p, units_word[n], len);
    }
}

static void number_to_words(idx_t n, char *buf, size_t *len)
{
    *len = 0;
    if (n == 0) return;
    /* millions are not needed for the problem domain (n <= 10000) */
    if (n >= 1000000) {          /* skip millions */
        idx_t millions = n / 1000000;
        add_group(millions, buf, len);
        add_str(buf, "million", len);
        n %= 1000000;
    }
    if (n >= 1000) {
        idx_t thousands = n / 1000;
        add_group(thousands, buf, len);
        add_str(buf, "thousand", len);
        n %= 1000;
    }
    add_group(n, buf, len);
}

seq_t a(idx_t n)
{
    /* Build the English word for n */
    char word[200];
    size_t word_len;
    number_to_words(n, word, &word_len);
    /* Filter allowed letters, converting U to V implicitly in roman_value */
    char filtered[200];
    size_t f_len = 0;
    for (size_t i = 0; i < word_len; ++i) {
        char c = word[i];
        switch (c) {
            case 'c': case 'd': case 'i':
            case 'l': case 'm': case 'u':
            case 'v': case 'x':
                filtered[f_len++] = c;
                break;
            default:
                break;
        }
    }
    filtered[f_len] = '\0';
    if (f_len == 0) return (seq_t)0;
    seq_t val = roman_value(filtered);
    return (val > 0) ? val : (seq_t)0;
}
