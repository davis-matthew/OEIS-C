/*
 * A179935  Squares where the number of decimal digits is also a square.
 * https://oeis.org/A179935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179935: squares where the number of decimal digits is a square.
   Generation: brute‑force search over integer roots, keeping those whose
   square has a digit count that is itself a perfect square. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool is_square_int(int v)
{
    if (v < 0) return false;
    int r = 0;
    while ((r + 1) * (r + 1) <= v) ++r;
    return r * r == v;
}

static int digit_len_seq(seq_t x)
{
    if (x == 0) return 1;
    int len = 0;
    seq_t t = x;
    while (t > 0) {
        ++len;
        t /= 10;
    }
    return len;
}

static void ensure_capacity(size_t needed, seq_t **p, size_t *cap)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newp = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (*p) {
        memcpy(newp, *p, (*cap) * sizeof(seq_t));
        free(*p);
    }
    *p    = newp;
    *cap  = new_cap;
}

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0) return 0; /* guard, not part of OEIS scheme */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_root = 0;

    size_t need = (size_t)n;
    ensure_capacity(need, &cache, &cache_cap);

    while (cache_len < n) {
        seq_t r = (seq_t)next_root;
        seq_t sq = r * r;
        int d = digit_len_seq(sq);
        if (is_square_int(d))
            cache[cache_len++] = sq;
        ++next_root;
    }

    return cache[n - 1];
}
