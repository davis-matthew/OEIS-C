/*
 * A210399  Number of (n+1)X4 0..3 arrays with every 2X2 subblock having one
 *          or three distinct values, and new values 0..3 introduced in row
 *          major order.
 * https://oeis.org/A210399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(k) = 65*a(k-1) -483*a(k-2) -7397*a(k-3) +54220*a(k-4)
 *            +315572*a(k-5) -1567384*a(k-6) -4952640*a(k-7) +15991008*a(k-8)
 *            +21762048*a(k-9) -51440128*a(k-10) -13774848*a(k-11)
 *            +33619968*a(k-12) for k>12 Initial terms: a(1)=549; a(2)=29647;
 *            a(3)=1596154; a(4)=85979301; a(5)=4631193134; a(6)=249456180...
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;   /* number of terms currently stored */

static const int coeff[12] =
{
    65,          -483,      -7397,      54220,
    315572,    -1567384,   -4952640,   15991008,
    21762048,   -51440128, -13774848,  33619968
};

static const char *init_str[12] =
{
    "549",
    "29647",
    "1596154",
    "85979301",
    "4631193134",
    "249456180709",
    "13436785945910",
    "723763304576749",
    "38985016148233742",
    "2099901274580672917",
    "113109748262417625254",
    "6092579354615210957373"
};

static seq_t parse_decimal(const char *s)
{
    seq_t v = 0;
    while (*s) {
        v = v * 10 + (seq_t)(*s - '0');
        ++s;
    }
    return v;
}

static void init_cache(void)
{
    if (cache_len != 0) return;
    cache_cap = 12;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);
    for (int i = 0; i < 12; ++i)
        cache[i] = parse_decimal(init_str[i]);
    cache_len = 12;
}

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 12;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) exit(1);
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_cache();
    size_t idx_n = (size_t)n;
    if (idx_n > cache_len) {
        ensure_capacity(idx_n);
        for (size_t idx = cache_len; idx < idx_n; ++idx) {
            seq_t pos_sum = 0;
            seq_t neg_sum = 0;
            for (int k = 0; k < 12; ++k) {
                seq_t term = cache[idx - 1 - k];
                int c = coeff[k];
                if (c >= 0) {
                    seq_t prod = mul_small(term, (unsigned int)c);
                    pos_sum += prod;
                } else {
                    seq_t prod = mul_small(term, (unsigned int)(-c));
                    neg_sum += prod;
                }
            }
            cache[idx] = pos_sum - neg_sum;
        }
        cache_len = idx_n;
    }
    return cache[n - 1];
}
