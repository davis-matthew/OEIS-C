/*
 * A037199  Number of consonants in n (in German).
 * https://oeis.org/A037199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of consonants in the German word for an integer.
   The algorithm follows the standard German spelling rules for
   integers (units, teens, tens, hundreds, thousands) and counts
   the consonant letters. */

#include <stdint.h>   /* for uint64_t used in idx_t */

static const unsigned char units[10] = {
    3, /* null  */
    2, /* eins  */
    2, /* zwei  */
    2, /* drei  */
    2, /* vier  */
    3, /* fünf  */
    4, /* sechs */
    3, /* sieben*/
    3, /* acht  */
    2  /* neun  */
};

static const unsigned char teens[10] = {
    3, /* zehn   */
    2, /* elf    */
    4, /* zwölf  */
    5, /* dreizehn */
    5, /* vierzehn */
    6, /* fünfzehn */
    6, /* sechzehn */
    5, /* siebzehn */
    6, /* achtzehn */
    5  /* neunzehn */
};

static const unsigned char tens_counts[10] = {
    0, 0, /* placeholders for 0 and 1 */
    5, /* zwanzig   */
    4, /* dreißig   */
    4, /* vierzig   */
    5, /* fünfzig   */
    5, /* sechzig   */
    4, /* sieb*ezig  (siebzig) */
    5, /* achtzig   */
    4  /* neunzig   */
};

#define HUNDRED_CONSONANTS     5   /* "hundert"    */
#define EINHUNDRED_CONSONANTS  6   /* "einhundert"*/
#define TAUSEND_CONSONANTS     4   /* "tausend"    */

/* Count consonants for numbers less than 100. */
static seq_t count_less100(idx_t n)
{
    if (n < 10) {
        return (seq_t)units[n];
    }
    if (n < 20) {
        return (seq_t)teens[n - 10];
    }

    /* 20 <= n < 100 */
    idx_t tens  = n / 10;
    idx_t unit  = n % 10;
    seq_t res   = (seq_t)tens_counts[tens];

    if (unit > 0) {
        /* "ein" is used instead of "eins" when it precedes a tens word */
        seq_t unitcnt = (unit == 1) ? 1 : (seq_t)units[unit];
        res += unitcnt + 2; /* "+2" for "und" */
    }
    return res;
}

/* Count consonants for numbers less than 1000 (does not handle 0). */
static seq_t count_below_1000(idx_t n)
{
    if (n == 0) return 0;

    idx_t hundreds = n / 100;
    idx_t rem      = n % 100;
    seq_t res;

    if (hundreds == 0) {
        res = 0;
    } else if (hundreds == 1) {
        res = (rem == 0) ? HUNDRED_CONSONANTS
                         : EINHUNDRED_CONSONANTS;
    } else {
        res = (seq_t)units[hundreds] + HUNDRED_CONSONANTS;
    }

    if (rem > 0) res += count_less100(rem);
    return res;
}

/* Count consonants for the thousands group */
static seq_t count_thousands_group(idx_t group)
{
    if (group == 1) {
        /* The special word for 1000 is "tausend" only */
        return TAUSEND_CONSONANTS;
    }
    return count_below_1000(group) + TAUSEND_CONSONANTS;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)units[0]; /* "null" */

    if (n < 1000) return count_below_1000(n);

    idx_t thousands = n / 1000;
    idx_t rest      = n % 1000;

    seq_t res = count_thousands_group(thousands);

    if (rest > 0) res += count_below_1000(rest);
    return res;
}
