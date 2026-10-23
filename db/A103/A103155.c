/*
 * A103155  Each letter appears an even number of times in the English names
 *          for 1 through n taken together (names with "and").
 * https://oeis.org/A103155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   toggle_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer such that all letters in the
   English names for 1 through k (with “and”) appear an even number
   of times in total.  Computed by incremental search. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/toggle_word.h"

static const char * const uni_names[20] = {
    0, "one", "two", "three", "four", "five", "six", "seven", "eight",
    "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};

static const char * const ten_names[8] = {
    "twenty", "thirty", "forty", "fifty", "sixty",
    "seventy", "eighty", "ninety"
};

/* Adds the parity of the English name of n into *mask.  n is assumed
   to be >0.  This follows British usage with “and”. */
static void add_parity_uint64(uint32_t *mask, unsigned long long n) {
    if (n >= 1000000ULL) {                /* millions and above */
        unsigned long long millions = n / 1000000ULL;
        add_parity_uint64(mask, millions);
        toggle_word("million", mask);
        unsigned long long rem = n % 1000000ULL;
        if (rem > 0) {
            if (rem < 100) {
                toggle_word("and", mask);
                add_parity_uint64(mask, rem);
            } else
                add_parity_uint64(mask, rem);
        }
        return;
    }
    if (n >= 1000ULL) {                   /* thousands */
        unsigned long long thousands = n / 1000ULL;
        add_parity_uint64(mask, thousands);
        toggle_word("thousand", mask);
        unsigned long long rem = n % 1000ULL;
        if (rem > 0) {
            if (rem < 100) {
                toggle_word("and", mask);
                add_parity_uint64(mask, rem);
            } else
                add_parity_uint64(mask, rem);
        }
        return;
    }
    if (n >= 100ULL) {                    /* hundreds */
        unsigned long long hundreds = n / 100ULL;
        add_parity_uint64(mask, hundreds);
        toggle_word("hundred", mask);
        unsigned long long rem = n % 100ULL;
        if (rem > 0) {
            toggle_word("and", mask);
            add_parity_uint64(mask, rem);
        }
        return;
    }
    if (n >= 20ULL) {                     /* twenties, etc. */
        unsigned long long tens = n / 10ULL;
        toggle_word(ten_names[tens - 2], mask);
        unsigned long long rem = n % 10ULL;
        if (rem > 0) {
            toggle_word(uni_names[rem], mask);
        }
        return;
    }
    if (n >= 1ULL) {                      /* 1–19 */
        toggle_word(uni_names[n], mask);
        return;
    }
    /* n == 0 never occurs in this sequence; included for completeness. */
}

/* The sequence generator.  It keeps a static cache of found terms to
   allow arbitrary access order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* initial offset is 1, so 0 is unused */

    static seq_t *sol = NULL;
    static size_t sol_cap = 0, sol_used = 0;
    static unsigned long long last_checked = 0;      /* last number processed */
    static uint32_t parity = 0;    /* current parity mask */

    /* Grow or initialise the solution array if needed. */
    if (sol_used < n && sol_cap == 0) {
        sol_cap = 8;
        sol = (seq_t *)malloc(sol_cap * sizeof(seq_t));
    }

    /* Continue processing numbers until we have at least n solutions. */
    while (sol_used < n) {
        ++last_checked;
        add_parity_uint64(&parity, last_checked);

        if (parity == 0) {          /* all letters even */
            if (sol_used == sol_cap) {
                sol_cap *= 2;
                sol = (seq_t *)realloc(sol, sol_cap * sizeof(seq_t));
            }
            sol[sol_used++] = (seq_t)last_checked;
        }
    }

    return sol[n - 1];
}
