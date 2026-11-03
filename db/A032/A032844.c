/*
 * A032844  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032844: Numbers whose base-6 digits alternate up/down
 * starting with "<".  Computed by searching sequentially until the
 * required index is reached.  A static cache of already generated
 * terms ensures efficient random‑order access. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* return true if value has the alternating up/down pattern in base 6
 * starting with "<" (i.e., d(m) < d(m-1) > d(m-2) < ...). */
static bool valid_base6(seq_t value) {
    unsigned char digits[64];
    idx_t len = 0;
    seq_t tmp = value;
    while (tmp > (seq_t)0) {
        digits[len++] = (unsigned char)(tmp % (seq_t)6);
        tmp /= (seq_t)6;
    }
    if (len == 0) return false;            /* 0 is not used */

    /* most significant digit is digits[len-1] */
    idx_t ms = len - 1;
    for (idx_t p = ms; p > (idx_t)0; --p) {
        /* comparison direction alternates with distance from MS digit */
        if (((ms - p) & (idx_t)1) == 0) {    /* expect "<" */
            if (!(digits[p] < digits[p - 1])) return false;
        } else {                              /* expect ">" */
            if (!(digits[p] > digits[p - 1])) return false;
        }
    }
    return true;
}

/* sequence a(n) */
seq_t a(idx_t n) {
    /* static cache holds the sequence values already generated */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of terms stored  */
    static idx_t cache_cap   = 0;      /* allocated capacity      */
    static seq_t next_candidate = (seq_t)1;   /* number to test next     */

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        if (valid_base6(next_candidate)) {
            if (cache_size == cache_cap) {
                idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - (idx_t)1];
}
