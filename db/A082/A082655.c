/*
 * A082655  Number of distinct letters needed to spell English names of
 *          numbers 1 through n.
 * https://oeis.org/A082655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = number of distinct letters used in the English names of
 *            the integers 1 … n where each number is written in the
 *            conventional US style without “and” or hyphens (e.g. 21 →
 *            “twenty one”, 1003 → “one thousand three”). Only the letters a–z
 *            are counted; any other character is ignored. The set of...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  Dependencies and type definitions                          */
/* ------------------------------------------------------------ */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/*  Global state (static to preserve across calls)             */
/* ------------------------------------------------------------ */
static bool seen_letters[26] = {0};
static seq_t cur_distinct  = 0;           /* current count of distinct letters */
static seq_t *dp = NULL;                 /* cumulative results for 1 … cur_max */
static size_t cur_max = 0;                /* maximum n already processed */

/* ------------------------------------------------------------ */
/*  Lookup tables for English words                           */
/* ------------------------------------------------------------ */
static const char *UNITS[20] = {
    "", "one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

static const char *TENS[10] = {
    "", "", "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

static const char *SCALES[10] = {
    "", "thousand","million","billion","trillion","quadrillion",
    "quintillion","sextillion","septillion","octillion"
};

/* ------------------------------------------------------------ */
/*  Helpers to add letters from a word                          */
/* ------------------------------------------------------------ */
static inline void add_letter(char c)
{
    if ('A' <= c && c <= 'Z')
        c = c - 'A' + 'a';
    if ('a' <= c && c <= 'z') {
        unsigned idx = (unsigned)(c - 'a');
        if (!seen_letters[idx]) {
            seen_letters[idx] = true;
            cur_distinct++;
        }
    }
}

static void add_word(const char *w)
{
    while (*w) add_letter(*w++);
}

/* ------------------------------------------------------------ */
/*  Build words for numbers 1 … 999 (no scales)                 */
/* ------------------------------------------------------------ */
static void add_small_group(idx_t n)
{
    if (n >= 100) {
        unsigned h = (unsigned)(n / 100);
        unsigned rem = (unsigned)(n % 100);
        add_word(UNITS[h]);          /* one…nine */
        add_word("hundred");         /* hundred */
        if (rem) add_small_group(rem);
    } else if (n >= 20) {
        unsigned t = (unsigned)(n / 10);
        unsigned rem = (unsigned)(n % 10);
        add_word(TENS[t]);           /* twenty…ninety */
        if (rem) add_word(UNITS[rem]);
    } else if (n >= 1) {
        add_word(UNITS[n]);          /* one…nineteen */
    }
}

/* ------------------------------------------------------------ */
/*  Build words for an arbitrary integer n (>=1)                 */
/* ------------------------------------------------------------ */
static void add_number_letters(idx_t n)
{
    if (n == 0) {
        add_word("zero");
        return;
    }

    /* process groups of 3 digits from low to high */
    unsigned group_idx = 0;
    while (n > 0) {
        unsigned group = (unsigned)(n % 1000);
        if (group > 0) {
            add_small_group(group);
            if (group_idx > 0) {
                add_word(SCALES[group_idx]); /* thousand, million, … */
            }
        }
        n /= 1000;
        group_idx++;
    }
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                      */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safe */

    /* Ensure dp can hold up to n */
    if (n > cur_max) {
        size_t new_size = (size_t)n;
        /* Resize dp with realloc */
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – fall back to recomputing completely */
            free(dp);
            dp = NULL;
            cur_max = 0;
            /* reset letter state */
            for (int i = 0; i < 26; ++i) seen_letters[i] = false;
            cur_distinct = 0;
            /* recompute whole range up to n */
        } else {
            dp = tmp;
        }

        /* Incrementally compute new entries from cur_max+1 to n */
        for (size_t i = cur_max + 1; i <= new_size; ++i) {
            add_number_letters((idx_t)i);
            dp[i - 1] = (seq_t)cur_distinct;
        }
        cur_max = new_size;
    }

    /* If cur_max < n due to allocation failure, recompute directly */
    if (cur_max < n) {
        /* reset state and recompute from scratch */
        for (int i = 0; i < 26; ++i) seen_letters[i] = false;
        cur_distinct = 0;
        cur_max = 0;
        if (dp) { free(dp); dp = NULL; }

        for (size_t i = 1; i <= n; ++i) {
            add_number_letters((idx_t)i);
            /* we can store in a temporary dynamic array if needed */
        }
        /* no dp stored, so just return result */
        return cur_distinct;
    }

    return dp[n - 1];
}
