/*
 * A228062  Numbers not expressible as a*b + b*c + a*c + 1 with positive
 *          numbers a, b, c.
 * https://oeis.org/A228062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   Numbers not expressible as a*b + b*c + a*c + 1 with positive
 *            integers a, b, c
 * Range:     u8 n <= 17, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

/* static cache of already computed sequence values */
static seq_t *seq_vals = NULL;     /* array of sequence elements (C) */
static size_t seq_cap  = 0;        /* allocated capacity of seq_vals (C) */
static idx_t  seq_len  = 0;        /* number of elements stored in seq_vals (C) */
static idx_t  sieve_lim = 0;       /* current maximum value sieved (C) */

/* generate all representable numbers up to |limit| and build seq_vals up to that limit */
static void generate_up_to(idx_t limit)
{
    size_t sz = (size_t)(limit + 1);
    bool *rep = (bool*)calloc(sz, sizeof(bool));
    if (!rep) return;                       /* out of memory – caller will not get a correct result */

    /* mark all numbers that can be written as a*b + b*c + a*c + 1 */
    for (idx_t a = 1; a <= limit; ++a) {
        for (idx_t b = a; b <= limit; ++b) {   /* use symmetry a <= b */
            unsigned __int128 ab = (unsigned __int128)a * b;
            if (ab > limit) break;
            unsigned __int128 minimal = ab + a + b + 1;   /* value with c = 1 */
            if (minimal > limit) break;
            idx_t max_c = (idx_t)((limit - (idx_t)ab - 1) / (a + b));
            for (idx_t c = 1; c <= max_c; ++c) {
                idx_t val = (idx_t)(ab + (unsigned __int128)(a + b) * c + 1);
                rep[val] = 1;
            }
        }
    }

    /* rebuild seq_vals with non‑representable numbers up to limit */
    free(seq_vals);
    seq_vals = NULL;
    seq_cap = 0;
    seq_len = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (!rep[i]) {
            if (seq_len >= seq_cap) {
                seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                seq_vals = (seq_t*)realloc(seq_vals, seq_cap * sizeof(seq_t));
            }
            seq_vals[seq_len++] = (seq_t)i;
        }
    }

    free(rep);
    sieve_lim = limit;
}

/* public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined behaviour for n == 0; return 0 */

    /* ensure we have at least n terms in seq_vals */
    if (seq_vals == NULL) {
        sieve_lim = 1024;          /* initial sieve limit */
        generate_up_to(sieve_lim);
    }
    while (seq_len < n) {
        sieve_lim = (sieve_lim == 0) ? 1024 : sieve_lim * 2;   /* grow the sieve */
        generate_up_to(sieve_lim);
    }
    return seq_vals[n - 1];
}
