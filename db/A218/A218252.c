/*
 * A218252  Start with 1. For each term m, the next term is the smallest
 *          positive integer k such that k - (sum of base 2 digits of k) = m.
 *          If no such k exists, use the smallest natural number not already
 *          in the sequence.
 * https://oeis.org/A218252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(1) = 1;
 * For n > 1 let m = a(n‑1). If there exists k such that
 * k - popcount(k) = m, then a(n) is the smallest positive integer
 * unused so far that satisfies this equation;
 * otherwise a(n) is the smallest unused natural number.
 * The sequence is a permutation of the positive integers.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* seq_t, idx_t are supplied by the build environment */

/* Count the number of 1‑bits of a seq_t value. */
static unsigned int popcount_seq(seq_t x)
{
    unsigned int cnt = 0;
    while (x) {
        cnt += (unsigned int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* ------------------------------------------------------------------ */
/* Memoisation state                                               */
/* ------------------------------------------------------------------ */
static idx_t        computed_to = 0;   /* largest n already computed */
static seq_t      *seq_arr     = NULL; /* array of computed a(n)      */
static size_t      seq_cap     = 0;    /* capacity of seq_arr          */

static uint8_t    *used_arr    = NULL; /* byte array marking used ints */
static size_t      used_cap    = 0;    /* capacity of used_arr         */

static seq_t      next_unused = 1;          /* candidate for next unused  */

/* ------------------------------------------------------------------ */
/* Utility helpers                                                 */
/* ------------------------------------------------------------------ */
static void ensure_seq_cap(idx_t need)
{
    if ((size_t)need <= seq_cap) return;
    size_t new_cap = (size_t)need;
    seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

static void ensure_used_cap(seq_t high)
{
    if (used_cap > (size_t)high) return;
    size_t new_cap = used_cap ? used_cap : 8;
    while ((size_t)high + 1 > new_cap) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used_arr, new_cap);
    if (tmp) {
        if (used_arr) {
            memset(tmp + used_cap, 0, new_cap - used_cap);
        } else {
            memset(tmp, 0, new_cap);
        }
        used_arr = tmp;
        used_cap = new_cap;
    }
}

static void mark_used(seq_t val)
{
    ensure_used_cap(val);
    used_arr[(size_t)val] = 1;
    if ((size_t)val >= (size_t)next_unused) {
        while (next_unused < used_cap && used_arr[(size_t)next_unused])
            ++next_unused;
    }
}

static seq_t next_term(seq_t m)
{
    seq_t k = m + 1;              /* candidates start just above m   */
    while (1) {
        seq_t f = k - (seq_t)popcount_seq(k);
        if (f == m) {
            if ((size_t)k < used_cap && used_arr[(size_t)k]) {
                /* already used – keep searching */
            } else {
                return k;
            }
        } else if (f > m) {
            /* no solution; use the smallest unused integer   */
            ensure_used_cap(next_unused);
            while (next_unused < used_cap && used_arr[(size_t)next_unused])
                ++next_unused;
            return next_unused;
        }
        ++k;
    }
}

/* ------------------------------------------------------------------ */
/* Public function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, defensive return */

    /* Grow memoisation storage if needed. */
    while (computed_to < n) {
        ++computed_to;
        ensure_seq_cap(computed_to);
        if (computed_to == 1) {
            seq_arr[0] = (seq_t)1;   /* a(1) = 1 */
            mark_used((seq_t)1);
        } else {
            seq_t prev = seq_arr[computed_to - 2];
            seq_t val  = next_term(prev);
            seq_arr[computed_to - 1] = val;
            mark_used(val);
        }
    }
    return seq_arr[n - 1];
}
