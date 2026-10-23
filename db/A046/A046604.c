/*
 * A046604  First denominator and then numerator of the 1/5-Pascal triangle
 *          (by row), excluding 1's.
 * https://oeis.org/A046604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = sequence of denominators and numerators (skipping 1's) of
 *            the 1/5‑Pascal triangle rows in order; for a fraction d/n with d
 *            in {1,5} we output d first; if d==5 we then output n (unless
 *            n==1). The triangle is defined by rows: row 1: [1] row 2: [1,1]
 *            row 3: [1, 1/5, 1] For n>=4: row n+1[i] =...
 * Range:     u8 n <= 67, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Reduced fraction with denominator 1 or 5. */
typedef struct { seq_t num; unsigned den; } frac_t;

/* Reduce a sum of two fractions.  Assumes den1, den2 ∈ {1,5}. */
static inline frac_t add_frac(frac_t a, frac_t b)
{
    frac_t r;
    if (a.den == b.den) {
        r.den = a.den;
        r.num = a.num + b.num;
    } else {
        /* one denominator is 1, the other 5 */
        if (a.den == 1) {
            r.num = a.num * (seq_t)5 + b.num;
        } else {
            r.num = b.num * (seq_t)5 + a.num;
        }
        r.den = 5;
    }

    /* reduce (num/5) if divisible by 5 */
    if (r.den == 5 && r.num % (seq_t)5 == (seq_t)0) {
        r.num /= (seq_t)5;
        r.den = 1;
    }
    return r;
}

/* Cached sequence values */
static seq_t *seq_cache      = NULL;
static size_t seq_cache_cap  = 0;
static size_t seq_cache_len  = 0;

/* Current source row */
static frac_t *prev_row      = NULL;
static size_t prev_size      = 0;

/* Append a value to the cache, growing if necessary. */
static void cache_append(seq_t val)
{
    if (seq_cache_len >= seq_cache_cap) {
        seq_cache_cap = seq_cache_cap == 0 ? 32 : seq_cache_cap * 2;
        seq_cache = (seq_t*)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
    }
    seq_cache[seq_cache_len++] = val;
}

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    /* --- initialization: row 1, row 2, row 3 --- */
    if (prev_row == NULL) {
        /* create row 3 explicitly */
        prev_size = 3;
        prev_row = (frac_t*)malloc(prev_size * sizeof(frac_t));
        prev_row[0].num = 1; prev_row[0].den = 1;
        prev_row[1].num = 1; prev_row[1].den = 5;
        prev_row[2].num = 1; prev_row[2].den = 1;

        seq_cache_cap = 32;
        seq_cache = (seq_t*)malloc(seq_cache_cap * sizeof(seq_t));
        seq_cache_len = 0;

        /* process interior of row 3 */
        frac_t *f = &prev_row[1];
        if (f->den == 1) {
            if (f->num != 1) cache_append(f->num);
        } else { /* f->den == 5 */
            cache_append(5);
            if (f->num != 1) cache_append(f->num);
        }
    }

    /* generate rows until cache has enough terms */
    while (seq_cache_len < (size_t)n) {
        size_t next_size = prev_size + 1;
        frac_t *next_row = (frac_t*)malloc(next_size * sizeof(frac_t));
        /* edges */
        next_row[0].num = 1; next_row[0].den = 1;
        next_row[next_size-1].num = 1; next_row[next_size-1].den = 1;

        /* interior */
        for (size_t i = 0; i + 1 < prev_size; ++i) {
            next_row[i+1] = add_frac(prev_row[i], prev_row[i+1]);
        }

        /* insert non‑1 entries into cache */
        for (size_t i = 1; i + 1 < next_size; ++i) {
            frac_t *f = &next_row[i];
            if (f->den == 1) {
                if (f->num != 1) cache_append(f->num);
            } else { /* f->den == 5 */
                cache_append(5);
                if (f->num != 1) cache_append(f->num);
            }
        }

        free(prev_row);
        prev_row = next_row;
        prev_size = next_size;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1, guard */
    ensure_cache(n);
    return seq_cache[n-1];
}
