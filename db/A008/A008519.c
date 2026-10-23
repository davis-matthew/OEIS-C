/*
 * A008519  Numbers whose American English name contains the letter 'o'.
 * https://oeis.org/A008519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose American English name contains the letter 'o' */
/* a(1)=0, a(2)=1, ... */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper arrays for words up to 999. */
static const char *const ones[20] = {
    "zero","one","two","three","four","five",
    "six","seven","eight","nine","ten",
    "eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};

static const char *const tens[10] = {
    "", "", "twenty","thirty","forty",
    "fifty","sixty","seventy","eighty","ninety"
};

/* Build the English word for 0 ≤ x < 1000. */
static void build_word_1000(int x, char *buf)
{
    buf[0] = '\0';
    if (x < 20) {
        strcpy(buf, ones[x]);
        return;
    }
    if (x < 100) {
        strcpy(buf, tens[x / 10]);
        int r = x % 10;
        if (r > 0) {
            strcat(buf, " ");
            strcat(buf, ones[r]);
        }
        return;
    }
    /* 100 … 999 */
    int h = x / 100;
    char tmp[64];
    strcpy(tmp, ones[h]);
    strcat(tmp, " hundred");
    strcpy(buf, tmp);
    int rem = x % 100;
    if (rem > 0) {
        strcat(buf, " ");
        if (rem < 20) {
            strcat(buf, ones[rem]);
        } else {
            strcat(buf, tens[rem / 10]);
            int r = rem % 10;
            if (r > 0) {
                strcat(buf, " ");
                strcat(buf, ones[r]);
            }
        }
    }
}

/* Test whether the word for n < 1000 contains the letter 'o'. */
static bool has_o_small(int n)
{
    char buf[128];
    build_word_1000(n, buf);
    for (char *p = buf; *p; ++p)
        if (*p == 'o')
            return true;
    return false;
}

/* Precomputed list of numbers ≤ 999 that contain 'o'. */
static seq_t prelist[1000];
static idx_t pre_count = 0;
static bool pre_initialized = false;

/* Initialise the prelist. */
static void init_prelist(void)
{
    if (pre_initialized)
        return;
    for (int i = 0; i < 1000; ++i) {
        if (has_o_small(i))
            prelist[pre_count++] = (seq_t)i;
    }
    pre_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_prelist();

    if (n <= pre_count) {
        return prelist[n - 1];
    } else {
        /* All numbers ≥ 1000 contain 'o' because of the magnitude word. */
        return (seq_t)(n + (idx_t)(999 - pre_count));
    }
}
