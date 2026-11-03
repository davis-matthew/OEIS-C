/*
 * A216589  Numbers n which don't have a preimage for A216556, i.e., such that
 *          A216587(n)=-1.
 * https://oeis.org/A216589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216589: numbers that contain either
 *  "00", "20", "30", … , "90", or a substring "11", "12", … , "19",
 *  or the single number 0.
 * This implementation scans successive integers, keeps a cache of the
 * first n matching numbers, and returns the n-th term.
 * The cache is expanded lazily, so calls may arrive in any order.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return true if x is excluded (i.e. belongs to the sequence). */
static bool is_excluded(seq_t x)
{
    /* 0 itself is part of the sequence. */
    if (x == (seq_t)0) return true;

    /* Store digits LSB → MSB. */
    uint8_t d[256];
    size_t l = 0;
    seq_t t = x;
    while (t != (seq_t)0) {
        d[l++] = (uint8_t)(t % (seq_t)10);
        t /= (seq_t)10;
    }

    /* Scan adjacent digit pairs from most significant to least.
       left  – the more significant digit of the pair,
       right – the next less significant digit. */
    for (size_t i = l; i > 1; --i) {
        uint8_t left  = d[i - 1];
        uint8_t right = d[i - 2];

        /* Substrings "20", "30", … , "90" */
        if (left >= 2 && left <= 9 && right == 0) return true;

        /* Substrings "11", "12", … , "19" */
        if (left == 1 && right >= 1 && right <= 9) return true;

        /* Substring "00" */
        if (left == 0 && right == 0) return true;
    }
    return false;
}

/* Return the n-th term of A216589 (n >= 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache of found terms */
    static size_t cap   = 0;      /* capacity of cache */
    static size_t sz    = 0;      /* number of terms stored */
    static seq_t cur    = (seq_t)0; /* candidate to test */

    /* Expand the cache until it contains at least n terms. */
    while (sz < (size_t)n) {
        if (is_excluded(cur)) {
            /* Ensure capacity for one more element. */
            size_t needed = sz + 1;
            grow_seq(&cache, &cap, needed);
            cache[sz++] = cur;
        }
        cur++;
    }
    return cache[(size_t)n - 1];
}
