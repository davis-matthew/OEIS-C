/*
 * A028168  Expansion of 1/((1-5x)(1-6x)(1-7x)(1-11x)).
 * https://oeis.org/A028168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 29*a(n-1) - 305*a(n-2) + 1387*a(n-3) - 2310*a(n-4) *
 *            Initial values: a(0)=1, a(1)=29, a(2)=536, a(3)=8086
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t mul_seq(seq_t a, seq_t b) {
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
    } else {
        return a * b;
    }
}
static inline seq_t add_seq(seq_t a, seq_t b) {
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((unsigned __int128)a + (unsigned __int128)b);
    } else {
        return a + b;
    }
}
static inline seq_t sub_seq(seq_t a, seq_t b) {
    if (sizeof(seq_t) <= 8) {
        return (seq_t)((unsigned __int128)a - (unsigned __int128)b);
    } else {
        return a - b;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t computed = 0;

    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    if (computed == 0) {
        if (cap >= 1) cache[0] = (seq_t)1;
        if (cap >= 2) cache[1] = (seq_t)29;
        if (cap >= 3) cache[2] = (seq_t)536;
        if (cap >= 4) cache[3] = (seq_t)8086;
        computed = 4;
    }

    if (n < 4) {
        return cache[(size_t)n];
    }

    if ((size_t)n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            seq_t t1 = mul_seq(cache[(size_t)(i-1)], (seq_t)29);
            seq_t t2 = mul_seq(cache[(size_t)(i-2)], (seq_t)305);
            seq_t t3 = mul_seq(cache[(size_t)(i-3)], (seq_t)1387);
            seq_t t4 = mul_seq(cache[(size_t)(i-4)], (seq_t)2310);
            seq_t sum_pos = add_seq(t1, t3);
            seq_t sum_neg = add_seq(t2, t4);
            cache[(size_t)i] = sub_seq(sum_pos, sum_neg);
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
