/*
 * A079051  Recamán variation: a(0) = 0; for n >= 1, a(n) = a(n-1) - f(n) if
 *          that number is positive and not already in the sequence, otherwise
 *          a(n) = a(n-1) + f(n), where f(n) = floor(sqrt(n)) (A000196).
 * https://oeis.org/A079051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 5141, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079051 – Recamán variation
 *
 * a(0) = 0
 * for n ≥ 1:
 *   let f(n) = floor(sqrt(n))
 *   if a(n‑1) – f(n) is positive and not already in the sequence,
 *       a(n) = a(n‑1) – f(n)
 *   otherwise
 *       a(n) = a(n‑1) + f(n)
 */

#include <stdlib.h>
#include <stdbool.h>

/* forward declaration of integer square root for idx_t */
static idx_t int_sqrt(idx_t x);

/* dynamic array to cache computed terms */
static seq_t *terms = NULL;    /* array of a(0)…a(current-1) */
static size_t used = 0;        /* number of terms stored */
static size_t capacity = 0;    /* allocated capacity */

/* grow the terms array if needed */
static void ensure_capacity(size_t need) {
    if (need <= capacity)
        return;
    size_t newcap = capacity ? capacity * 2 : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    terms = tmp;
    capacity = newcap;
}

/* linear search for membership – optimal for n ≤ 10 000 */
static bool used_value(seq_t v, size_t upto) {
    for (size_t i = 0; i < upto; ++i)
        if (terms[i] == v)
            return true;
    return false;
}

/* compute a(n) for all n up to requested value */
static void compute_upto(idx_t n) {
    /* note: idx_t may be larger than size_t; cast is safe for small n */
    while (used <= (size_t)n) {
        if (used == 0) {            /* a(0) = 0 */
            terms[0] = (seq_t)0;
            used = 1;
            continue;
        }
        seq_t prev = terms[used - 1];
        idx_t idx   = (idx_t)used;          /* current n to compute */
        idx_t fidx  = int_sqrt(idx);        /* floor(sqrt(n)) as idx_t */
        seq_t f = (seq_t)fidx;

        /* try a(n) = a(n‑1) – f(n) if possible */
        if (f <= prev) {                       /* subtraction possible */
            seq_t cand = prev - f;
            if (cand > (seq_t)0 && !used_value(cand, used))
                terms[used] = cand;            /* use the subtraction */
            else
                terms[used] = prev + f;        /* otherwise use addition */
        } else {
            terms[used] = prev + f;            /* f > prev → forced addition */
        }
        ++used;
    }
}

seq_t a(idx_t n) {
    ensure_capacity((size_t)n + 1);
    compute_upto(n);
    return terms[n];
}

/* integer square root for idx_t (floor of sqrt) */
static idx_t int_sqrt(idx_t x) {
    idx_t low = 0;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {          /* avoid division by zero */
            low = 1;
            continue;
        }
        if (mid <= x / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}
