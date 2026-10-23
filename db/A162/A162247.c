/*
 * A162247  Irregular triangle in which row n lists all factorizations of n,
 *          sorted by the number of factors in each factorization.
 * https://oeis.org/A162247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, tabf, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2912, u16 n <= 9513, u32 n <= 9513, u64 n <= 9513,
 *            u128 n <= 9513, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A162247 - flattened list of all
   factorizations of row n, sorted by #factors then lexicographically.
   a(1) is the 1st element of this list. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    idx_t  len;     /* number of factors */
    idx_t *data;    /* array of the factors (each <= n) */
} Part;

/* Global cache for the flattened sequence */
static seq_t *flat      = NULL;  /* allocated array of terms */
static size_t flat_cap   = 0;    /* capacity of flat */
static size_t flat_len   = 0;    /* current number of terms */

/* Last row that has already been produced */
static idx_t last_row_processed = 1;

/* Ensure that flat has room for at least 'need' more terms */
static void
ensure_flat_cap(idx_t need)
{
    if (flat_len + need > flat_cap) {
        size_t new_cap = flat_cap ? flat_cap * 2 : 64;
        while (new_cap < flat_len + need)
            new_cap *= 2;
        flat = (seq_t *)realloc(flat, new_cap * sizeof(seq_t));
        flat_cap = new_cap;
    }
}

/* Comparator used with qsort to order parts by length then lexicographically */
static int
compare_parts(const void *a, const void *b)
{
    const Part *pa = (const Part *)a;
    const Part *pb = (const Part *)b;
    if (pa->len < pb->len) return -1;
    if (pa->len > pb->len) return 1;
    for (idx_t i = 0; i < pa->len; ++i) {
        if (pa->data[i] < pb->data[i]) return -1;
        if (pa->data[i] > pb->data[i]) return 1;
    }
    return 0;
}

/* Append a part to the parts array, reallocating if necessary */
static void
add_part(Part **parts, size_t *len, size_t *cap, idx_t *data, idx_t dlen)
{
    if (*len == *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 4;
        *parts = (Part *)realloc(*parts, new_cap * sizeof(Part));
        *cap   = new_cap;
    }
    Part *p = &(*parts)[*len];
    p->len  = dlen;
    p->data = data;
    (*len)++;
}

/* Recursively generate all factorisations of rem,
   appending each as a part to 'parts'.  The current
   partial factorisation is stored in curr of length curr_len.
   Factors are always in nondecreasing order. */
static void
rec(idx_t rem, idx_t min_f,
    idx_t *curr, idx_t curr_len,
    Part **parts, size_t *parts_len, size_t *parts_cap)
{
    for (idx_t f = min_f; f <= rem / f; ++f) {      /* f divides rem ? */
        if (rem % f == 0) {
            idx_t rem2 = rem / f;
            if (rem2 >= f) {
                /* Create a part consisting of curr + f + rem2 */
                idx_t *data = (idx_t *)malloc((curr_len + 2) * sizeof(idx_t));
                for (idx_t i = 0; i < curr_len; ++i)
                    data[i] = curr[i];
                data[curr_len]     = f;
                data[curr_len + 1] = rem2;
                add_part(parts, parts_len, parts_cap, data, curr_len + 2);
            }
            /* Continue building longer factorizations */
            curr[curr_len] = f;
            rec(rem2, f, curr, curr_len + 1, parts, parts_len, parts_cap);
        }
    }
}

/* Generate all factorizations for a single row m,
   sort them, and append their factors to the global flat array. */
static void
generate_row(idx_t m)
{
    Part *parts   = NULL;
    size_t parts_cap = 0, parts_len = 0;

    /* Trivial factorisation m itself */
    idx_t *data = (idx_t *)malloc(sizeof(idx_t));
    data[0] = m;
    add_part(&parts, &parts_len, &parts_cap, data, 1);

    /* Generate all non‑trivial factorizations */
    idx_t curr[64];   /* depth is <= 13 for m <= 10000 */
    rec(m, 2, curr, 0, &parts, &parts_len, &parts_cap);

    /* Sort the parts by length then lexicographically */
    qsort(parts, parts_len, sizeof(Part), compare_parts);

    /* Total number of new terms */
    idx_t total_new = 0;
    for (size_t i = 0; i < parts_len; ++i)
        total_new += parts[i].len;

    ensure_flat_cap(total_new);

    /* Append each factorisation's factors to flat */
    for (size_t i = 0; i < parts_len; ++i) {
        for (idx_t j = 0; j < parts[i].len; ++j)
            flat[flat_len++] = (seq_t)parts[i].data[j];
        free(parts[i].data);
    }
    free(parts);
}

/* Public function to obtain a(n) */
seq_t a(idx_t n)
{
    while (flat_len < n) {
        generate_row(last_row_processed);
        last_row_processed++;
    }
    return flat[n - 1];
}
