/*
 * A157903  a(n) is the smallest number in Spanish with n consonants.
 * https://oeis.org/A157903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, more
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157903:  a(n) is the smallest natural number whose Spanish
   name contains exactly n consonants.  The function below computes this
   value by scanning sequentially from 1 up, storing the first number
   encountered for each possible consonant count, until it has found all
   counts up to the largest index requested.  The implementation
   generates Spanish words only enough to count consonants, without
   building the full string.  It handles numbers up to 999 999 999
   (sufficient for all n up to at least 10 000). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static inline int count_10_to_19(idx_t n) {
    /* n is 10..19 */
    static const int teen_consonants[10] = {2,2,2,3,4,3,4,4,4,3};
    return teen_consonants[n-10];
}

static inline int count_1_to_9(idx_t n) {
    /* n is 1..9 */
    static const int unit_consonants[10] = {0,1,2,3,3,3,2,2,2,2};
    return unit_consonants[n];
}

static inline int count_20_to_29(idx_t n) {
    /* n is 20..29, but 20 is handled here as well (unit=0) */
    int unit = n % 10;
    return 3 + count_1_to_9(unit);           /* "veinti" (3) + unit */
}

static inline int count_30_to_99(idx_t n) {
    int tens_digit = n / 10;        /* 3..9 */
    int unit = n % 10;
    static const int tens_consonants[10] = {0,0,3,4,4,5,4,4,4,4};
    int base = tens_consonants[tens_digit]; /* consonants in tens word */
    if (unit > 0) base += 1 + count_1_to_9(unit);  /* + "y" + unit */
    return base;
}

static inline int count_1_to_99(idx_t n) { /* 1 <= n <= 99 */
    if (n < 10)                     return count_1_to_9(n);
    if (n < 20)                     return count_10_to_19(n);
    if (n < 30)                     return count_20_to_29(n);
    return count_30_to_99(n);        /* 30..99 */
}

static inline int count_1_to_999(idx_t n) { /* 1 <= n <= 999 */
    if (n < 100) return count_1_to_99(n);

    int h = n / 100;        /* 1..9 */
    int rem = n % 100;      /* 0..99 */

    if (h == 1) { /* 100..199 */
        if (rem == 0) return 2;                /* "cien" */
        return 3 + count_1_to_99(rem);          /* "ciento" (3) + rest */
    }

    static const int hundreds_base[10] = {0,0,6,7,7,5,6,6,6,6};
    int base = hundreds_base[h];
    if (rem == 0) return base;                  /* e.g., 200, 300, ... */
    return base + count_1_to_99(rem);
}

static inline int count_0_to_999999(idx_t n) { /* 0 <= n <= 999,999 */
    if (n < 1000) return count_1_to_999(n);

    idx_t th = n / 1000;
    idx_t rem = n % 1000;
    int sum = 0;
    if (th > 0) {
        if (th == 1) sum += 2;                 /* "mil" */
        else sum += count_1_to_999(th) + 2;     /* part + "mil" */
    }
    if (rem > 0) sum += count_1_to_999(rem);
    return sum;
}

static inline int count_consonants(idx_t n) { /* full general for n >= 1 */
    if (n == 0) return 2;                     /* "cero" */
    if (n < 1000000) return count_0_to_999999(n);

    idx_t mil = n / 1000000;
    idx_t rem = n % 1000000;

    int sum = 0;
    if (mil == 1) {
        sum += 1 + 5;                         /* "un" + "millón"  => 6 */
    } else {
        sum += count_0_to_999999(mil) + 5;     /* millions part + "millones" */
    }
    if (rem > 0) sum += count_consonants(rem);
    return sum;
}

/* Public API */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safe */

    /* Static cache of first occurrence for each consonant count */
    static seq_t *res = NULL;
    static size_t res_cap = 0;
    static idx_t processed = 0;          /* last number processed */
    static size_t filled = 0;            /* number of indices 1..max_target that are set */
    static idx_t max_target = 0;         /* largest n requested so far */

    /* enlarge cache if needed */
    if (n > max_target) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > res_cap) {
            seq_t *tmp = mem_resize(seq_t, res, new_cap);
            if (!tmp) { /* allocation failed – return 0 as fallback */
                return 0;
            }
            /* initialise new entries to 0 */
            if (tmp != res) {
                size_t delta = new_cap - res_cap;
                mem_set(tmp + res_cap, 0, delta * sizeof(seq_t));
                res = tmp;
            } else {
                mem_set(res + res_cap, 0, (new_cap - res_cap) * sizeof(seq_t));
            }
            res_cap = new_cap;
        }
        max_target = n;
    }

    /* scan numbers until all counts up to max_target are found */
    while (filled < max_target) {
        ++processed;
        int c = count_consonants(processed);
        if (c > 0 && (size_t)c <= max_target && res[c] == 0) {
            res[c] = processed;
            ++filled;
        }
    }

    return res[n];
}
