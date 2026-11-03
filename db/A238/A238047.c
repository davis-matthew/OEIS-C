/*
 * A238047  a(n) is the least number larger than the total number of letters
 *          in the Spanish names for all terms up to and including a(n).
 * https://oeis.org/A238047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A238047: a(n) is the least integer m such that       \
 * m > S_{n-1} + len(name(m)), where S_{n-1} is the sum of      \
 * lengths of the Spanish names of a(1) … a(n-1).           *
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Lengths of Spanish words for numbers 1 … 999 (positive). */
static int len_num_small(idx_t n)
{
    /* unit lengths: 1..9  */
    static const int unit_len[10] = {0,3,3,4,6,5,4,5,4,5};
    /* lengths for 1..15 */
    static const int len_1_to_15[16] = {
        0, 3,3,4,6,5,4,5,4,5,4,4,4,5,7,6
    };
    /* dieci units for 16-19 (1..4) */
    static const int dieci_unit_len[5] = {0,4,5,4,5};

    if (n == 0)
        return 0;

    if (n <= 15)
        return len_1_to_15[n];

    if (n <= 19)                 /* 16-19: dieci + unit */
        return 5 + dieci_unit_len[n - 15];

    if (n == 20)
        return 6;   /* veinte */

    if (n < 30)                 /* 21-29: veinti + unit */
        return 6 + unit_len[n - 20];

    if (n < 100) {              /* 30-99 */
        int tens = n / 10;
        int units = n % 10;
        int base_len;
        switch (tens) {
            case 3: base_len = 7; break;   /* treinta */
            case 4: base_len = 8; break;   /* cuarenta */
            case 5: base_len = 9; break;   /* cincuenta */
            case 6: case 7: case 8: case 9:
                base_len = 7; break;         /* sesenta, setenta, ochenta, noventa */
            default: base_len = 0; break;
        }
        if (units == 0)
            return base_len;
        return base_len + 3 + unit_len[units];   /* + ' y ' = 3 chars, but here not used in thousands */
    }

    /* 100-999 */
    int hundreds = n / 100;
    int rem = n % 100;
    if (hundreds == 0)
        return len_num_small(rem);

    if (hundreds == 1) {              /* 100-199 */
        if (rem == 0)
            return 4;                 /* cien */
        return 6 + 1 + len_num_small(rem);  /* 'ciento ' + rest */
    }

    /* 200-900 */
    static const int hundreds_len[10] = {0,0,10,11,13,10,11,11,11,11};
    if (rem == 0)
        return hundreds_len[hundreds];
    return hundreds_len[hundreds] + 1 + len_num_small(rem);
}

/* Full length of a positive integer (may be > 1000). */
static int len_num(idx_t n)
{
    /* thousands */
    if (n >= 1000) {
        idx_t thousands = n / 1000;
        idx_t rem = n % 1000;

        int len = 0;
        if (thousands == 1)
            len = 3;                               /* "mil" */
        else
            len = len_num(thousands) + 1 + 3;      /* number + space + "mil" */

        if (rem == 0)
            return len;

        return len + 1 + len_num(rem);            /* + space */
    }

    /* < 1000 */
    return len_num_small(n);
}

/* Cache for computed terms */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static seq_t cumulative_sum = 0;
static idx_t last_term = 0;

/* Ensure cache capacity */
static void ensure_cache_cap(idx_t needed)
{
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed) new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Extend cache up to requested index */
    while (cache_len < n) {
        idx_t m = last_term + 1;   /* start search from previous term + 1 */
        int len;
        for (;;) {
            len = len_num(m);
            if ((seq_t)m > cumulative_sum + (seq_t)len)
                break;
            ++m;
        }
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = (seq_t)m;
        cumulative_sum += (seq_t)len;
        last_term = m;
        ++cache_len;
    }
    return cache[n - 1];
}
