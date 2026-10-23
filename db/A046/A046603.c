/*
 * A046603  First numerator and then denominator of each element of the
 *          1/5-Pascal triangle (by row), excluding 1's.
 * https://oeis.org/A046603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) is the n‑th integer obtained by * listing, for each row r ≥
 *            2 of the 1/5‑Pascal triangle, * the numerator then the
 *            denominator of each inner entry * (entries on the edges are 1/1
 *            and are omitted). The triangle * is formed by adding two
 *            fractions from the previous row and * reducing by gcd. T...
 * Range:     u8 n <= 31, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

typedef struct {
    seq_t num;
    seq_t den;
} Rational;

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static Rational *row_prev = NULL;
static Rational *row_curr = NULL;
static size_t cap_prev = 0;
static size_t cap_curr = 0;

static idx_t last_generated_row = 0;   /* index of the last fully built row */

/* Ensure cache can hold at least 'needed' elements */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a value to the cache */
static void push_cache(seq_t v)
{
    ensure_cache(cache_len + 1);
    cache[cache_len++] = v;
}

/* Ensure a rational array can hold at least 'need' elements */
static void ensure_row(Rational **arr, size_t *cap, idx_t need)
{
    if (*cap >= (size_t)need) return;
    size_t new_cap = *cap ? *cap * 2 : 1;
    while (new_cap < (size_t)need) new_cap *= 2;
    *arr = (Rational*)realloc(*arr, sizeof(Rational) * new_cap);
    *cap = new_cap;
}

/* Build rows until the cache contains at least n terms */
static void build_until(idx_t n)
{
    /* Lazy initialization */
    if (!cache) {
        cache = (seq_t*)malloc(64 * sizeof(seq_t));
        cache_cap = 64;
        cache_len = 0;
    }

    if (!row_prev) {
        row_prev = (Rational*)malloc(sizeof(Rational));
        cap_prev = 1;
        row_prev[0].num = (seq_t)1;
        row_prev[0].den = (seq_t)1;
        last_generated_row = 0; /* row 0 is already in row_prev */
    }

    while (cache_len < (size_t)n) {
        idx_t next_row = last_generated_row + 1; /* 1, 2, 3, ... */

        /* Ensure current row buffer is large enough */
        ensure_row(&row_curr, &cap_curr, next_row + 1);

        if (next_row == 1) {
            /* Row 1: edges only */
            row_curr[0].num = row_curr[0].den = (seq_t)1;
            row_curr[1].num = row_curr[1].den = (seq_t)1;
            /* no inner terms → no output */
        }
        else if (next_row == 2) {
            /* Row 2: special entry 1/5 */
            row_curr[0].num = row_curr[0].den = (seq_t)1;
            row_curr[1].num = (seq_t)1;
            row_curr[1].den = (seq_t)5;
            row_curr[2].num = row_curr[2].den = (seq_t)1;

            /* Process inner fraction 1/5 */
            if (row_curr[1].num != (seq_t)1) push_cache(row_curr[1].num);
            if (row_curr[1].den != (seq_t)1) push_cache(row_curr[1].den);
        }
        else {
            /* General row generation via rational addition */
            row_curr[0].num = row_curr[0].den = (seq_t)1;
            row_curr[next_row].num = row_curr[next_row].den = (seq_t)1;

            for (idx_t k = 1; k < next_row; ++k) {
                seq_t n1 = row_prev[k - 1].num;
                seq_t d1 = row_prev[k - 1].den;
                seq_t n2 = row_prev[k].num;
                seq_t d2 = row_prev[k].den;

                seq_t num = n1 * d2 + n2 * d1;
                seq_t den = d1 * d2;

                seq_t g = gcd(num, den);
                num /= g;
                den /= g;

                row_curr[k].num = num;
                row_curr[k].den = den;

                if (num != (seq_t)1) push_cache(num);
                if (den != (seq_t)1) push_cache(den);
            }
        }

        /* Prepare for next iteration: swap rows */
        Rational *tmp = row_prev;
        row_prev = row_curr;
        row_curr = tmp;

        size_t tmp_cap = cap_prev;
        cap_prev = cap_curr;
        cap_curr = tmp_cap;

        last_generated_row = next_row;
    }
}

/* Public interface required by the test harness */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n==0, return 0 */

    build_until(n);
    return cache[n - 1];
}
