/*
 * A039413  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 6's.
 * https://oeis.org/A039413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose decimal * representation
 *            contains an equal positive number of the digits * 2 and 6. The
 *            function generates terms by linear search and * memoises them so
 *            later calls are fast
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- helpers ---------- */
static bool has_equal_2_and_6(seq_t v) {
    int cnt2 = 0, cnt6 = 0;
    seq_t tmp = v;
    while (tmp > (seq_t)0) {
        seq_t d = tmp % (seq_t)10;
        if (d == (seq_t)2) cnt2++;
        else if (d == (seq_t)6) cnt6++;
        tmp = tmp / (seq_t)10;
    }
    return (cnt2 > 0) && (cnt6 > 0) && (cnt2 == cnt6);
}

static void ensure_capacity(idx_t needed, size_t *cap, seq_t **ptr) {
    if ((idx_t)*cap >= needed)
        return;
    size_t newcap = *cap ? *cap : 1;
    while ((idx_t)newcap < needed)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
    if (tmp == NULL && newcap > 0) abort();
    *ptr = tmp;
    *cap = newcap;
}

/* ---------- main function ---------- */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t last_val = 0;

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for offset 1, but safe */
    if (n <= cache_len)
        return cache[n - 1];

    ensure_capacity(n, &cache_cap, &cache);

    seq_t val = last_val;
    while (cache_len < n) {
        val = val + (seq_t)1;
        if (has_equal_2_and_6(val)) {
            cache[cache_len++] = val;
        }
    }
    last_val = val;
    return cache[n - 1];
}
