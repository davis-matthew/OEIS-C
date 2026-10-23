/*
 * A014403  Numbers found in denominators of expansion of Airy function Bi(x).
 * https://oeis.org/A014403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = a(2k-2) * 3k * (3k-1) (k ≥ 1, a(0)=1) a(2k+1) = a(2k-1)
 *            * 3k * (3k+1) (k ≥ 2, a(1)=1, a(3)=4) These recurrences generate
 *            the denominators of the Bi(x) series as listed in OEIS A014403
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *f_cache = NULL;
static size_t f_caps = 0;
static size_t f_len = 0;   /* number of f elements computed (last index+1) */

static seq_t *g_cache = NULL;
static size_t g_caps = 0;
static size_t g_len = 0;   /* number of g elements computed (last index+1) */

/* Ensure f[k] exists for all k ≤ target */
static void f_ensure(idx_t target)
{
    if (f_len == 0) {
        f_cache = (seq_t *)malloc(sizeof(seq_t));
        f_caps = 1;
        f_cache[0] = (seq_t)1;  /* f(0) */
        f_len = 1;
    }
    while (f_len <= target) {
        idx_t curr = (idx_t)f_len;          /* current k to compute */
        seq_t mult = (seq_t)(3 * curr);     /* 3k */
        mult *= (seq_t)(3 * curr - 1);      /* 3k-1 */
        if (f_len >= f_caps) {
            size_t new_cap = f_caps * 2;
            seq_t *tmp = (seq_t *)realloc(f_cache, new_cap * sizeof(seq_t));
            f_cache = tmp;
            f_caps = new_cap;
        }
        f_cache[f_len] = f_cache[f_len - 1] * mult;
        f_len++;
    }
}

/* Ensure g[k] exists for all k ≤ target */
static void g_ensure(idx_t target)
{
    if (g_len == 0) {
        g_cache = (seq_t *)malloc(sizeof(seq_t));
        g_caps = 1;
        g_cache[0] = (seq_t)1;  /* g(0) */
        g_len = 1;
    }
    while (g_len <= target) {
        idx_t curr = (idx_t)g_len; /* current k to compute */
        seq_t val;
        if (curr == 1) {
            val = (seq_t)4;      /* special case a(3)=4 (k=1) */
        } else {
            seq_t mult = (seq_t)(3 * curr);   /* 3k */
            mult *= (seq_t)(3 * curr + 1);    /* 3k+1 */
            val = g_cache[g_len - 1] * mult;
        }
        if (g_len >= g_caps) {
            size_t new_cap = g_caps * 2;
            seq_t *tmp = (seq_t *)realloc(g_cache, new_cap * sizeof(seq_t));
            g_cache = tmp;
            g_caps = new_cap;
        }
        g_cache[g_len] = val;
        g_len++;
    }
}

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* floor(n/2) */
    if ((n & 1ULL) == 0) {     /* even index -> f(k) */
        f_ensure(k);
        return f_cache[k];
    } else {                   /* odd index -> g(k) */
        g_ensure(k);
        return g_cache[k];
    }
}
