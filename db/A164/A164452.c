/*
 * A164452  Number of binary strings of length n with no substrings equal to
 *          0001 0010 or 1101.
 * https://oeis.org/A164452
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-4) - 2*a(n-5) + a(n-6)
 *            * Offset: 4 (a(4) is the first term) * Initial values: a(4)=13,
 *            a(5)=21, a(6)=32, a(7)=48, a(8)=70, a(9)=101 * For n<4 the
 *            function returns 0 (value undefined by OEIS)
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 63, u64 n <= 131,
 *            u128 n <= 269, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t   cap   = 0;      /* allocated size of cache   */
static idx_t   computed_max = 3; /* greatest n for which a(n) is known in cache */

/* Ensure cache is large enough and initialise new entries up to n.  */
static void ensure_capacity(idx_t n) {
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 64 : n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* out of memory */
        cache = new_cache;
        cap = new_cap;
    }
    /* Initialise any new slots up to n.  */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i < 4) {
            cache[i] = 0;            /* undefined values below offset */
        } else if (i == 4) {
            cache[i] = 13;
        } else if (i == 5) {
            cache[i] = 21;
        } else if (i == 6) {
            cache[i] = 32;
        } else if (i == 7) {
            cache[i] = 48;
        } else if (i == 8) {
            cache[i] = 70;
        } else if (i == 9) {
            cache[i] = 101;
        } else {
            /* compute using recurrence: avoid multiplications to stay within seq_t bounds */
            seq_t sum_pos = cache[i-1] + cache[i-1] + cache[i-1];    /* 3*a(i-1)   */
            sum_pos += cache[i-3];
            sum_pos += cache[i-4];
            sum_pos += cache[i-6];
            seq_t sum_neg = cache[i-2] + cache[i-2] + cache[i-2];    /* 3*a(i-2)   */
            sum_neg += cache[i-5];
            sum_neg += cache[i-5];
            cache[i] = sum_pos - sum_neg;   /* guaranteed non‑negative */
        }
    }
    computed_max = (n > computed_max) ? n : computed_max;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* undefined below offset */
    ensure_capacity(n);
    /* If n has not yet been computed within ensure_capacity, compute it. */
    if (n > computed_max) {
        ensure_capacity(n); /* compute missing terms up to n */
    }
    return cache[n];
}
