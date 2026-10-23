/*
 * A026421  a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *          1<=i<j<=n, with initial terms 3,4.
 * https://oeis.org/A026421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer > a(n-1) and not a(i)*a(j) for
 *            1<=i<j<=n, a(1)=3, a(2)=4
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *seq_arr = NULL;      /* array of terms */
static size_t  seq_len = 0;        /* number of stored terms */
static size_t  seq_cap = 0;        /* allocated capacity for seq_arr */

static uint8_t *present = NULL;    /* boolean array marking which values have appeared */
static size_t  present_cap = 0;    /* allocated capacity for present array */

/* ensure seq_arr has room for 'needed' elements */
static void ensure_seq_cap(size_t needed)
{
    if (needed > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* ensure present array can index values up to 'needed' */
static void ensure_present_cap(size_t needed)
{
    if (needed + 1 > present_cap) {
        size_t new_cap = present_cap ? present_cap * 2 : 128;
        while (new_cap < needed + 1) new_cap *= 2;
        present = (uint8_t *)realloc(present, new_cap * sizeof(uint8_t));
        for (size_t i = present_cap; i < new_cap; ++i) present[i] = 0;
        present_cap = new_cap;
    }
}

/* add a new term to the sequence */
static void add_term(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq_arr[seq_len++] = val;
    size_t v = (size_t)val;          /* value assumed to fit in size_t */
    ensure_present_cap(v);
    present[v] = 1;
}

/* check whether x is equal to a product of two earlier terms */
static bool isForbidden(seq_t x)
{
    seq_t sqrtx = floor_sqrt(x);
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t ai = seq_arr[i];
        if (ai > sqrtx) break;
        if (x % ai == 0) {
            seq_t j = x / ai;
            if (j == ai) continue;          /* same factor, not allowed (i<j) */
            if (j <= present_cap - 1 && present[(size_t)j])
                return true;
        }
    }
    return false;
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    /* initialise sequence with the first two terms on first call */
    if (seq_len == 0) {
        add_term((seq_t)3);
        add_term((seq_t)4);
    }

    /* compute terms up to required index */
    while (seq_len < (size_t)n) {
        seq_t prev = seq_arr[seq_len - 1];
        seq_t cand = prev + 1;
        while (isForbidden(cand)) cand++;
        add_term(cand);
    }

    return seq_arr[(size_t)n - 1];
}
