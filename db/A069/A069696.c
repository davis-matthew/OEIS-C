/*
 * A069696  Triangular numbers with internal digits 4.
 * https://oeis.org/A069696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the n-th triangular number T(k) whose decimal
 *            representation has all internal digits equal to 4. For n = 1..14
 *            the triangular numbers themselves are accepted because they have
 *            fewer than three digits. From n ≥ 15 only triangular numbers
 *            with all internal digits equal to 4 are chosen
 * Range:     u8 n <= 14, u16 n <= 17, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: full, pass, 18 terms through n = 18 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a decimal number has all internal digits equal to 4.
   Numbers with fewer than three digits are accepted automatically. */
static bool internal_digits_all4(seq_t x)
{
    /* 1‑ or 2‑digit numbers: no internal digits. */
    if (x < (seq_t)10) return true;
    if (x < (seq_t)100) return true;

    /* Count digits of x. */
    idx_t d = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        tmp /= 10;
        ++d;
    }

    /* d >= 3 here. Compute 10^(d-1) to isolate the internal part. */
    seq_t pow10 = 1;
    for (idx_t i = 1; i < d; ++i) pow10 *= 10;

    /* Remove the first and last digits. */
    seq_t internal = (x / 10) % (pow10 / 10);   /* 10^(d-2) */

    /* Check all d‑2 internal digits, including zeros. */
    for (idx_t i = 0; i < d - 2; ++i) {
        if ((internal % 10) != 4) return false;
        internal /= 10;
    }
    return true;
}

/* Dynamic array holding the pre‑computed terms. */
static seq_t *terms      = NULL;
static size_t terms_size = 0;
static size_t terms_cap  = 0;

/* Append a new term, enlarging the array if necessary. */
static void add_term(seq_t v)
{
    if (terms_size == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) exit(1);            /* graceful abort on OOM */
        terms      = tmp;
        terms_cap  = new_cap;
    }
    terms[terms_size++] = v;
}

/* Build the known finite sequence up to a generous bound. */
static void init_sequence(void)
{
    if (terms) return;     /* already initialised */

    /* Last known triangular number is T(1043) = 544446.
       Choosing a higher bound (e.g., 20000) keeps the search trivial. */
    const idx_t N_MAX = 20000;
    for (idx_t k = 0; k <= N_MAX; ++k) {
        seq_t tri = ((seq_t)k * ((seq_t)k + 1)) / 2;
        if (internal_digits_all4(tri))
            add_term(tri);
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined, but harmless */

    init_sequence();

    /* Convert 1‑based index to 0‑based array position. */
    if ((size_t)(n - 1) >= terms_size)
        return (seq_t)0;                 /* beyond known terms */
    return terms[n - 1];
}
