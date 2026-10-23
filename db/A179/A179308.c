/*
 * A179308  List of positive integers, omitting any number n if another number
 *          with the same set of digits as n has already been listed.
 * https://oeis.org/A179308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 636, u32 n <= 1022, u64 n <= 1022,
 *            u128 n <= 1022, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179308: 
 * a(n) is the nth smallest integer that is the minimal number for a new set of digits
 * (excluding the empty set and the set {0}). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>   /* qsort */

/* A small structure to store a set of digits in sorted order. */
typedef struct {
    unsigned len;          /* number of digits */
    uint8_t digits[10];    /* digits in strictly increasing order */
} DigitVec;

/* Static array of all minimal numbers for each admissible digit set. */
static seq_t seq_arr[1022];
static bool seq_initialized = false;

/*
 * Helper to build the digit vector for a given mask.
 * mask 1 << d means digit d is present (0 <= d <= 9).
 * The mask never equals 0 or 1 (which would be the empty set or set {0}).
 */
static DigitVec build_vec(uint16_t mask)
{
    DigitVec v;
    v.len = 0;
    /* find the smallest non‑zero digit */
    int first = -1;
    for (int d = 1; d <= 9; ++d)
        if (mask & (1 << d)) { first = d; break; }

    /* first is guaranteed to be found because mask != 1 */
    v.digits[v.len++] = (uint8_t)first;

    /* append remaining digits in ascending order */
    for (int d = 0; d <= 9; ++d) {
        if (mask & (1 << d) && d != first)
            v.digits[v.len++] = (uint8_t)d;
    }
    return v;
}

/* Comparator for sorting digit vectors by the numerical value they represent. */
static int cmp_digitvec(const void *a, const void *b)
{
    const DigitVec *va = (const DigitVec *)a;
    const DigitVec *vb = (const DigitVec *)b;

    if (va->len != vb->len)
        return (va->len < vb->len) ? -1 : 1;

    for (unsigned i = 0; i < va->len; ++i) {
        if (va->digits[i] != vb->digits[i])
            return (va->digits[i] < vb->digits[i]) ? -1 : 1;
    }
    return 0;
}

/* Convert a digit vector into a seq_t value. */
static seq_t vec_to_value(const DigitVec *v)
{
    seq_t val = 0;
    for (unsigned i = 0; i < v->len; ++i)
        val = val * 10 + (seq_t)v->digits[i];
    return val;
}

/* One‑time initialization that fills seq_arr. */
static void seq_init(void)
{
    DigitVec vecs[1022];
    int pos = 0;
    for (uint16_t mask = 1; mask < (1 << 10); ++mask) {
        if (mask == (1 << 0))          /* skip set {0} */
            continue;
        vecs[pos++] = build_vec(mask);
    }

    /* pos should now be 1022 */
    qsort(vecs, (size_t)pos, sizeof(DigitVec), cmp_digitvec);

    for (int i = 0; i < pos; ++i)
        seq_arr[i] = vec_to_value(&vecs[i]);

    seq_initialized = true;
}

/* Public function: a(n) with offset 1. */
seq_t a(idx_t n)
{
    if (!seq_initialized)
        seq_init();

    if (n == 0 || n > 1022)
        return (seq_t)0;                /* undefined for out‑of‑range indices */

    return seq_arr[(size_t)(n - 1)];
}
