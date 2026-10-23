/*
 * A107772  a(1)=1, a(n) = smallest integer not previously used which contains
 *          a digit from a(n-1).
 * https://oeis.org/A107772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest unused integer that contains a digit from a(n-1)
 *            * with initial value a(1) = 1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function memoizes the sequence globally, so it can be called with
 * indices in any order. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* helper: return bitmask of digits occuring in x (bits 0–9). */
static uint16_t digit_mask(seq_t x)
{
    uint16_t mask = 0;
    /* treat zero specially */
    if (x == 0) return 1;
    /* cast to native integer for digit extraction */
    unsigned long long t = (unsigned long long)x;
    while (t != 0) {
        mask |= 1 << (t % 10);
        t /= 10;
    }
    return mask;
}

/* dynamic array of terms (1–based indexing). */
static seq_t *terms = NULL;           /* array of computed terms */
static idx_t  terms_capacity = 0;     /* allocated size */
static idx_t  terms_size = 0;         /* number of terms currently stored */

/* bitmap of used numbers. */
static uint8_t *used = NULL;          /* 0 = unused, 1 = used */
static idx_t    used_capacity = 0;    /* allocated size */

/* ensure that `terms` has capacity for at least `need` indices (+1 for 1‑based). */
static void ensure_terms(idx_t need)
{
    if (need + 1 <= terms_capacity) return;
    idx_t new_cap = terms_capacity ? terms_capacity * 2 : 16;
    if (new_cap < need + 1) new_cap = need + 1;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = new_terms;
    terms_capacity = new_cap;
}

/* ensure that `used` bitmap has capacity for at least `n` indices. */
static void ensure_used(idx_t n)
{
    if (n < used_capacity) return;
    idx_t new_cap = used_capacity ? used_capacity * 2 : 16;
    while (new_cap <= n) new_cap *= 2;
    uint8_t *new_used = (uint8_t *)realloc(used, new_cap);
    memset(new_used + used_capacity, 0, new_cap - used_capacity);
    used = new_used;
    used_capacity = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* out of bounds, undefined but safe */

    /* compute terms up to n if necessary */
    if (n <= terms_size) return terms[n];

    /* initialize structures on first call */
    if (terms_size == 0) {
        ensure_terms(1);
        terms[1] = 1;
        terms_size = 1;
        ensure_used(1);
        used[1] = 1;
    }
    /* previous term and its digit mask */
    seq_t prev = terms[terms_size];
    uint16_t prevMask = digit_mask(prev);

    /* compute subsequent terms */
    for (idx_t i = terms_size + 1; i <= n; ++i) {
        idx_t cand = 0;
        while (1) {
            ensure_used(cand);
            if (!used[cand] && (digit_mask((seq_t)cand) & prevMask)) break;
            ++cand;
        }
        seq_t val = (seq_t)cand;
        ensure_terms(i);
        terms[i] = val;
        ensure_used(cand);
        used[cand] = 1;
        prev = val;
        prevMask = digit_mask(val);
        terms_size = i;
    }
    return terms[n];
}
