/*
 * A147653  Number of letters (including spaces) in last name of n-th
 *          President of U.S.A.
 * https://oeis.org/A147653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of letters (including spaces) in last name of
 * the n-th President of the United States.
 * a(n) = length of the last name of the n-th president.
 * Indices are 1-based; for n beyond the last known president
 * the function returns 0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const char * const names[] = {
        "Washington",   /* 1 */
        "Adams",        /* 2 */
        "Jefferson",    /* 3 */
        "Madison",      /* 4 */
        "Monroe",       /* 5 */
        "Adams",        /* 6 */
        "Jackson",      /* 7 */
        "Van Buren",    /* 8 */
        "Harrison",     /* 9 */
        "Tyler",        /*10 */
        "Polk",         /*11 */
        "Taylor",       /*12 */
        "Fillmore",     /*13 */
        "Pierce",       /*14 */
        "Buchanan",     /*15 */
        "Lincoln",      /*16 */
        "Johnson",      /*17 */
        "Grant",        /*18 */
        "Hayes",        /*19 */
        "Garfield",     /*20 */
        "Arthur",       /*21 */
        "Cleveland",    /*22 */
        "Harrison",     /*23 */
        "Cleveland",    /*24 */
        "McKinley",     /*25 */
        "Roosevelt",    /*26 */
        "Taft",         /*27 */
        "Wilson",       /*28 */
        "Harding",      /*29 */
        "Coolidge",     /*30 */
        "Hoover",       /*31 */
        "Roosevelt",    /*32 */
        "Truman",       /*33 */
        "Eisenhower",   /*34 */
        "Kennedy",      /*35 */
        "Johnson",      /*36 */
        "Nixon",        /*37 */
        "Ford",         /*38 */
        "Carter",       /*39 */
        "Reagan",       /*40 */
        "Bush",         /*41 */
        "Clinton",      /*42 */
        "Bush",         /*43 */
        "Obama",        /*44 */
        "Trump",        /*45 */
        "Biden"         /*46 */
    };
    static const idx_t count = (idx_t)(sizeof(names)/sizeof(names[0]));
    if (n < 1 || n > count)
        return 0;

    const char *s = names[n - 1];
    seq_t len = 0;
    while (*s) {
        len++;
        s++;
    }
    return len;
}
