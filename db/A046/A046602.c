/*
 * A046602  First denominator and then numerator of each element of the
 *          1/5-Pascal triangle (by row).
 * https://oeis.org/A046602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn, frac, less
 * Formula:   a(n) = first denominator then numerator of each element of the *
 *            1/5-Pascal triangle, built by starting with 1/1. * Each
 *            following row has 1/1 at both ends. For an interior entry, * -
 *            if the two entries above are both 1/1, the new entry is 1/5; * -
 *            otherwise it is the sum of the two fractions in...
 * Range:     u8 n <= 60, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#include "../utils/gcd.h"   /* Provides: seq_gcd(seq_t, seq_t) or gcd(seq_t, seq_t) */


typedef struct Fraction {
    seq_t num;   /* numerator   */
    seq_t den;   /* denominator */
} Fraction;

/* Reduce a fraction to lowest terms.  The denominator is always >0. */
static void reduce_fraction(Fraction *f) {
    seq_t g = gcd(f->num, f->den);
    if (g != (seq_t)1) {
        f->num /= g;
        f->den /= g;
    }
}

/* Build a fraction from given numerator and denominator. */
static Fraction make_fraction(seq_t n, seq_t d) {
    Fraction f = { n, d };
    return f;
}

/* Add two fractions.  Special case: if both are 1/1, return 1/5. */
static Fraction add_fraction(const Fraction *a, const Fraction *b) {
    seq_t one  = (seq_t)1;
    seq_t five = (seq_t)5;

    if (a->num == one && a->den == one &&
        b->num == one && b->den == one) {
        /* The first non‑trivial interior entry is 1/5. */
        return make_fraction(one, five);
    }

    Fraction r;
    r.num = a->num * b->den + b->num * a->den;
    r.den = a->den * b->den;
    reduce_fraction(&r);
    return r;
}

/* Memoised storage for the sequence. */
static seq_t *seq_vals = NULL;          /* flattened sequence (den, num, den, num, ...) */
static size_t seq_cap  = 0;              /* capacity in seq_t elements */
static size_t seq_len  = 0;              /* filled length */

/* Current row of the triangle.  Only the previous row is needed. */
static Fraction *cur_row = NULL;         /* array of fractions */
static size_t cur_row_len = 0;           /* number of fractions in cur_row */

/* Helper to ensure capacity of the sequence array. */
static void ensure_seq_cap(size_t needed) {
    while (seq_len + needed > seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failed */
        seq_vals = tmp;
        seq_cap = new_cap;
    }
}

/* Ensure the triangle is generated up to the requested index n. */
static void extend_to(idx_t n) {
    seq_t one = (seq_t)1;
    /* Initialisation on first call. */
    if (!seq_vals) {
        /* First row: 1/1 */
        cur_row_len = 1;
        cur_row     = (Fraction *)malloc(sizeof(Fraction));
        cur_row[0]  = make_fraction(one, one);

        ensure_seq_cap(2);
        seq_vals[seq_len++] = one;   /* denominator */
        seq_vals[seq_len++] = one;   /* numerator  */
    }

    /* Grow row by row until we have enough terms. */
    while (seq_len < (size_t)n) {
        size_t new_len = cur_row_len + 1;
        Fraction *next = (Fraction *)malloc(new_len * sizeof(Fraction));

        /* Edges: 1/1 */
        next[0] = make_fraction(one, one);
        next[new_len - 1] = make_fraction(one, one);

        /* Interior terms */
        for (size_t j = 1; j < new_len - 1; ++j) {
            next[j] = add_fraction(&cur_row[j - 1], &cur_row[j]);
        }

        /* Append the new row to the sequence */
        ensure_seq_cap(new_len * 2);
        for (size_t j = 0; j < new_len; ++j) {
            seq_vals[seq_len++] = next[j].den;
            seq_vals[seq_len++] = next[j].num;
        }

        /* Advance to next row */
        free(cur_row);
        cur_row     = next;
        cur_row_len = new_len;
    }
}

/* External function as required by the build. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but keep return value non‑zero */
    extend_to(n);
    return seq_vals[n - 1];
}
