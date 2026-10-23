/*
 * A032860  Numbers whose base-5 representation Sum_{i=0..m} d(i)*5^i has d(m)
 *          > d(m-1) < d(m-2) > ...
 * https://oeis.org/A032860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate numbers whose base‑5 digits alternate
   d(m) > d(m-1) < d(m-2) > … starting from the most
   significant digit.  The numbers are produced in
   ascending order by a depth‑first search and cached
   so that any random access a(n) is fast. */
#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of computed terms          */
static size_t cache_cap = 0;    /* allocated capacity                */
static size_t cache_sz  = 0;    /* number of terms stored            */
static int    next_len  = 0;    /* length of next numbers to generate */

static void push_val(seq_t val) {
    if (cache_sz == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure is fatal */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

static void gen(int len, int pos, int prev, seq_t curr, int needGreater) {
    if (pos == len) {
        push_val(curr);
        return;
    }
    if (needGreater) {              /* need prev > next digit           */
        for (int d = 0; d < prev; ++d)
            gen(len, pos + 1, d, curr * 5 + d, 0);
    } else {                        /* need prev < next digit           */
        for (int d = prev + 1; d <= 4; ++d)
            gen(len, pos + 1, d, curr * 5 + d, 1);
    }
}

static void generate_length(int len) {
    if (len == 1) {
        for (int d = 0; d <= 4; ++d)
            push_val((seq_t)d);
    } else {
        for (int d = 1; d <= 4; ++d)
            gen(len, 1, d, (seq_t)d, 1);   /* next digit must be smaller */
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* defined only for n ≥ 1 */
    while (cache_sz < (size_t)n) {
        ++next_len;
        generate_length(next_len);
    }
    return cache[n - 1];
}
