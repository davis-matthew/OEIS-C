/*
 * A046588  Distinct numbers in writing first numerator and then denominator
 *          of each element to the right of the central elements of the
 *          1/4-Pascal triangle (by row).
 * https://oeis.org/A046588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) is the nth distinct integer that appears as either a *
 *            numerator or a denominator when, for each row of the 1/4‑Pascal
 *            * triangle, we write the numerators followed by the denominators
 *            of * all entries strictly to the right of the central element of
 *            that row. * * The triangle is generated as...
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_gcd, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_gcd.h"
#include "../utils/seq_mul.h"

typedef struct {
    seq_t num;
    seq_t den;
} frac_t;

/* ----------------------------------------------------------------------
 *  Hash set for seq_t values (open addressing, linear probing)
 * ---------------------------------------------------------------------- */
static size_t h_size  = 0;
static size_t h_count = 0;
static seq_t *h_keys  = NULL;
static unsigned char *h_used = NULL;
static const uint64_t H_PRIME = 11400714785074694791ULL; /* Knuth's constant */

static void h_init(void)
{
    h_size  = 64;
    h_keys  = (seq_t *)malloc(h_size * sizeof(seq_t));
    h_used  = (unsigned char *)calloc(h_size, 1);
    h_count = 0;
}

static void h_grow(void)
{
    size_t new_size = h_size << 1;
    seq_t *new_keys = (seq_t *)malloc(new_size * sizeof(seq_t));
    unsigned char *new_used = (unsigned char *)calloc(new_size, 1);

    for (size_t i = 0; i < h_size; ++i) {
        if (h_used[i]) {
            size_t pos = ((size_t)h_keys[i] * H_PRIME) & (new_size - 1);
            while (new_used[pos]) pos = (pos + 1) & (new_size - 1);
            new_used[pos] = 1;
            new_keys[pos] = h_keys[i];
        }
    }
    free(h_keys);
    free(h_used);
    h_keys = new_keys;
    h_used = new_used;
    h_size = new_size;
}

/* returns true if key was added (was not present before) */
static bool h_insert(seq_t key)
{
    size_t idx = ((size_t)key * H_PRIME) & (h_size - 1);

    while (h_used[idx]) {
        if (h_keys[idx] == key) return false;
        idx = (idx + 1) & (h_size - 1);
    }
    h_used[idx] = 1;
    h_keys[idx] = key;
    ++h_count;

    if (h_count > h_size / 2) h_grow();
    return true;
}

/* ----------------------------------------------------------------------
 *  Dynamic array for the sequence values
 * ---------------------------------------------------------------------- */
static size_t seq_cap = 0;
static size_t seq_len = 0;
static seq_t *seq_buf  = NULL;

static void seq_append(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 8 : seq_cap << 1;
        seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_buf[seq_len++] = v;
}

/* ----------------------------------------------------------------------
 *  Fraction operations
 * ---------------------------------------------------------------------- */
static inline void frac_add(const frac_t *a, const frac_t *b, frac_t *res)
{
    seq_t num1 = seq_mul(a->num, b->den);
    seq_t num2 = seq_mul(b->num, a->den);
    seq_t num  = num1 + num2;
    seq_t den  = seq_mul(a->den, b->den);

    seq_t g = seq_gcd(num, den);
    res->num = num / g;
    res->den = den / g;
}

/* ----------------------------------------------------------------------
 *  Global state retained across calls
 * ---------------------------------------------------------------------- */
static bool initialized = false;
static frac_t *curr_row = NULL;
static size_t curr_len = 0;

/* ----------------------------------------------------------------------
 *  Process a row: for every element strictly to the right of the central
 *  element, add its numerator and denominator to the sequence if they
 *  are new.
 * ---------------------------------------------------------------------- */
static void process_row(const frac_t *row, size_t len)
{
    size_t central = (len - 1) / 2;
    for (size_t i = central + 1; i < len; ++i) {
        if (h_insert(row[i].num)) seq_append(row[i].num);
        if (h_insert(row[i].den)) seq_append(row[i].den);
    }
}

/* ----------------------------------------------------------------------
 *  Main routine
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise hash and result array */
        h_init();
        seq_cap = 0;
        seq_len = 0;
        seq_buf = NULL;

        /* --- row 1 (index 1) ------------------------------------------------ */
        frac_t row1[3];
        for (size_t i = 0; i < 3; ++i) { row1[i].num = 1; row1[i].den = 1; }
        process_row(row1, 3);

        /* --- row 2 (index 2) ------------------------------------------------ */
        curr_row = (frac_t *)malloc(3 * sizeof(frac_t));
        curr_row[0].num = 1; curr_row[0].den = 1;
        curr_row[1].num = 1; curr_row[1].den = 4;
        curr_row[2].num = 1; curr_row[2].den = 1;
        curr_len = 3;

        initialized = true;
    }

    /* generate rows until we have at least n distinct numbers */
    while (seq_len < n) {
        /* process current row */
        process_row(curr_row, curr_len);

        /* generate next row */
        size_t next_len = curr_len + 1;
        frac_t *next_row = (frac_t *)malloc(next_len * sizeof(frac_t));

        next_row[0].num = 1; next_row[0].den = 1;
        for (size_t i = 1; i < curr_len; ++i)
            frac_add(&curr_row[i-1], &curr_row[i], &next_row[i]);
        next_row[next_len-1].num = 1; next_row[next_len-1].den = 1;

        free(curr_row);
        curr_row = next_row;
        curr_len = next_len;
    }

    return seq_buf[n-1];
}
