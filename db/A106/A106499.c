/*
 * A106499  Ordered perimeter of primitive integer-sided triangle one of whose
 *          angles is twice another.
 * https://oeis.org/A106499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 869, u64 n <= 869, u128 n <= 869,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation of OEIS A106499: ordered perimeter of primitive
   integer‑sided triangle one of whose angles is twice another.
   a(n) = s * (r + s) for coprime integers r, s with r < s < 2r
   and satisfying the strict triangle inequality  
   2*r*r + r*s > s*s. */

#include <stdlib.h>
#include <stdbool.h>

/* Comparison function for qsort */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Euclidean GCD for idx_t, return as seq_t */
static seq_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return (seq_t)a;
}

/* Dynamic array of perimeters */
static seq_t *terms   = NULL;
static idx_t  terms_cap = 0;
static idx_t  terms_len = 0;
static idx_t  last_r    = 0;      /* largest r already processed */

/* Ensure terms array has space for at least 'need' entries */
static void ensure_cap(idx_t need)
{
    while (terms_cap < need) {
        idx_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Add one perimeter to the array */
static void add_term(seq_t val)
{
    ensure_cap(terms_len + 1);
    terms[terms_len++] = val;
}

/* Generate perimeters for all r in (last_r, r_limit] */
static void generate_to(idx_t r_limit)
{
    for (idx_t r = last_r + 1; r <= r_limit; ++r) {
        for (idx_t s = r + 1; s < 2 * r; ++s) {
            if (gcd_idx(r, s) != 1) continue;

            /* strict triangle inequality 2*r*r + r*s > s*s */
            if ((seq_t)s * (seq_t)s >= (seq_t)r * (seq_t)s + 2 * (seq_t)r * (seq_t)r)
                continue;

            seq_t val = (seq_t)s * ((seq_t)r + (seq_t)s);
            add_term(val);
        }
    }
    last_r = r_limit;
}

/* Sort the array and remove duplicates */
static void uniq_sort(void)
{
    if (terms_len == 0) return;
    qsort(terms, terms_len, sizeof(seq_t), cmp_seq);

    idx_t new_len = 0;
    for (idx_t i = 0; i < terms_len; ++i) {
        if (new_len == 0 || terms[i] != terms[new_len - 1]) {
            terms[new_len++] = terms[i];
        }
    }
    terms_len = new_len;
}

/* Required exported function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but harmless */

    /* Keep generating batches until we have at least n terms */
    while (terms_len < n) {
        idx_t next_r = last_r + 512;       /* batch size */
        generate_to(next_r);
        uniq_sort();
    }
    return terms[n - 1];
}
