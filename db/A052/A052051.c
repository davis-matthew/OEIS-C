/*
 * A052051  Numbers k such that k^3 is a cube whose digits occur with an equal
 *          minimum frequency of 2.
 * https://oeis.org/A052051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   has_equal_min_freq_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A052051 – numbers k such that k³ has all non‑zero digit
 * frequencies equal and at least 2.  The implementation builds the
 * sequence lazily, caching already found terms so that any call to
 * a(idx_t n) for n ≥ 1 returns the correct value regardless of the
 * order of the calls.  All arithmetic is performed in seq_t; the
 * implementation works for any integer type defined by the build
 * (8, 16, 32, 64, 128 bits or an arbitrary–precision class). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_min_freq_two.h"

/* static state for on‑demand generation */
static seq_t *terms   = NULL;
static idx_t  tsize   = 0; /* number of terms stored */
static idx_t  tcap    = 0; /* capacity of terms[] */
static idx_t  next_k  = 1; /* next integer k to test */

/* internal helper: ensure array capacity for at least 'need' elements */
static void
ensure_capacity(idx_t need)
{
    if (need <= tcap)
        return;
    idx_t new_cap = tcap ? tcap << 1 : 1;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms)
        abort();                /* out of memory – terminate */
    terms = new_terms;
    tcap  = new_cap;
}

/* generate terms up to index 'n' (1‑based) */
static void
generate_up_to(idx_t n)
{
    ensure_capacity(n);
    while (tsize < n) {
        seq_t k  = (seq_t)next_k;
        seq_t cube = k * k * k;
        if (has_equal_min_freq_two(cube))
            terms[tsize++] = k;
        next_k++;
    }
}

/* public function required by the specification */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for 0, safe default */
    generate_up_to(n);
    return terms[n - 1];
}
