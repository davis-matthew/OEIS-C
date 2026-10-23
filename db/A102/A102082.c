/*
 * A102082  Number of times the n-th letter in an infinitely repeating English
 *          alphabet appears in the US English name for the number n.
 * https://oeis.org/A102082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of times the n‑th letter of an infinitely
 *            repeating alphabet appears in the US English name for n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   count_in_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_in_word.h"

/* Static tables of English words for numbers. */
static const char *const unit_words[20] = {
    "zero", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen",
    "nineteen"
};

static const char *const tens_words[10] = {
    "", "", "twenty", "thirty", "forty",
    "fifty", "sixty", "seventy", "eighty", "ninety"
};

/* Scale words for thousands and higher.  Index 0 is unused. */
static const char *const scale_names[7] = {
    "", "thousand", "million", "billion",
    "trillion", "quadrillion", "quintillion"
};

/* Count letter occurrences in a number < 1000. */
static seq_t count_less_than_thousand(idx_t n, char target)
{
    seq_t cnt = 0;
    if (n >= 100) {
        idx_t hundreds = n / 100;
        cnt += count_in_word(unit_words[hundreds], target);
        cnt += count_in_word("hundred", target);
        n %= 100;
    }
    if (n >= 20) {
        idx_t tens = n / 10;
        cnt += count_in_word(tens_words[tens], target);
        n %= 10;
    }
    if (n > 0 && n < 20) {
        cnt += count_in_word(unit_words[n], target);
    }
    return cnt;
}

/* Recursive helper: counts target letter in the English name of n. */
static seq_t count_number(idx_t n, char target)
{
    if (n == 0) return count_in_word("zero", target);

    /* Find the highest power of 1000 <= n. */
    idx_t tmp = n;
    idx_t pow = 1;
    int k = 0;
    while (tmp >= 1000) {
        tmp /= 1000;
        pow *= 1000;   /* pow = 1000^k after the loop */
        ++k;
    }

    if (k == 0) return count_less_than_thousand(n, target);

    idx_t high = n / pow;
    idx_t low  = n % pow;

    seq_t cnt = 0;
    if (high > 0) {
        cnt += count_number(high, target);
        cnt += count_in_word(scale_names[k], target);
    }
    if (low > 0) {
        cnt += count_number(low, target);
    }

    return cnt;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    char target = (n == 0) ? 'a'
                           : (char)('a' + (unsigned)((n - 1) % 26));
    return count_number(n, target);
}
