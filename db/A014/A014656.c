/*
 * A014656  Numbers of letters in n (in the Norwegian language Bokmål).
 * https://oeis.org/A014656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Norwegian Bokmål spelling of n
 *            (11 is spelled "elleve", 21 is spelled "tjueen", 101 is "to
 *            hundre og ett", etc., using "ett" as the remainder for thousands
 *            and hundreds)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t digit_len[10]   = {0,2,2,3,4,3,4,3,4,2};
static const seq_t teens[20]       = {0,0,0,0,0,0,0,0,0,0,2,6,4,7,7,6,7,6,5,6};
static const seq_t tens_len[10]    = {0,2,4,6,5,5,6,5,4,5};

static seq_t letter_count(idx_t n)
{
    if (n < 10) return digit_len[(int)n];
    if (n < 20) return teens[(int)n];
    if (n < 100) {
        idx_t t = (n / 10) * 10;
        idx_t u = n % 10;
        seq_t res = tens_len[(int)(t / 10)];
        if (u != 0) res += digit_len[(int)u];
        return res;
    }
    if (n < 1000) {
        idx_t h   = n / 100;
        idx_t rem = n % 100;
        seq_t res;
        if (h == 1) res = 6;                /* "hundre" */
        else        res = digit_len[(int)h] + 6;
        if (rem != 0) {
            res += 2;                       /* "og" */
            seq_t rem_len = letter_count(rem);
            if (rem == 1) rem_len += 1;    /* "ett" instead of "en" */
            res += rem_len;
        }
        return res;
    }
    /* thousands (n >= 1000) */
    idx_t thou = n / 1000;
    idx_t rem  = n % 1000;
    seq_t res;
    if (thou == 1)                /* "tusen" */
        res = 5;
    else {
        res = letter_count(thou);
        res += 5;                 /* "tusen" */
    }
    if (rem != 0) {
        res += 2;                  /* "og" */
        seq_t rem_len = letter_count(rem);
        if (rem == 1) rem_len += 1; /* "ett" instead of "en" */
        res += rem_len;
    }
    return res;
}

seq_t a(idx_t n)
{
    return letter_count(n);
}
