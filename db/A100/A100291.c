/*
 * A100291  Numbers of the form a^4 + b^3 with a, b > 0.
 * https://oeis.org/A100291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sorted unique values of a^4 + b^3 for a,b > 0
 * Range:     u8 none, u16 n <= 409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_impl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types are provided by the build system.  Do not
   redefine them. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_impl.h"

/* ------------------------------------------------------------------ */
/* Compare function for qsort. Unsuitable for arbitrary-precision
   classes without operator<, but the build environment guarantees
   that seq_t supports the usual comparison operators.                 */

/* ------------------------------------------------------------------ */
/* Static storage for the generated sequence.                           */
/* ------------------------------------------------------------------ */
static seq_t *seq_vals = NULL;
static size_t  seq_cap  = 0;
static size_t  seq_size = 0;

/* ------------------------------------------------------------------ */
/* Helper: append a value to the dynamic array.                       */
/* ------------------------------------------------------------------ */
static void push_value(seq_t val)
{
    if (seq_size >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failed – abort further work */
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_size++] = val;
}

/* ------------------------------------------------------------------ */
/* Initialise the sequence once.  Generates all a^4 + b^3 values up to */
/* an upper bound that guarantees more than 10 000 distinct terms.     */
/* ------------------------------------------------------------------ */
static void init_sequence(void)
{
    if (seq_vals) return;                /* already initialised */

    /* Choose a bound that comfortably exceeds the 10 000‑th term.
       200 000 000 is large enough and keeps the intermediate values
       within 32 bits so that no overflow occurs.                    */
    const seq_t UPPER = (seq_t)200000000u;

    /* Find the largest a such that a^4 <= UPPER. */
    size_t max_a = 0;
    for (size_t a = 1; ; ++a) {
        seq_t a4 = (seq_t)a * a * a * a;
        if (a4 > UPPER) break;
        max_a = a;
    }

    /* Find the largest b such that b^3 <= UPPER. */
    size_t max_b = 0;
    for (size_t b = 1; ; ++b) {
        seq_t b3 = (seq_t)b * b * b;
        if (b3 > UPPER) break;
        max_b = b;
    }

    /* Reserve space for all pair sums; the real number of distinct
       values will be smaller due to collisions. */
    seq_cap  = max_a * max_b + 1;
    seq_vals = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_size = 0;

    /* Enumerate all sums a^4 + b^3, a,b > 0. */
    for (size_t a = 1; a <= max_a; ++a) {
        seq_t a4 = (seq_t)a * a * a * a;
        for (size_t b = 1; b <= max_b; ++b) {
            seq_t b3 = (seq_t)b * b * b;
            push_value(a4 + b3);
        }
    }

    /* Sort the array. */
    qsort(seq_vals, seq_size, sizeof(seq_t), cmp_seq_impl);

    /* Deduplicate in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < seq_size; ++i) {
        if (uniq == 0 || seq_vals[i] != seq_vals[uniq - 1]) {
            seq_vals[uniq++] = seq_vals[i];
        }
    }
    seq_size = uniq;
}

/* ------------------------------------------------------------------ */
/* Public function: return the n‑th term (1‑based index).              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for 0, but harmless */
    init_sequence();
    if ((size_t)n > seq_size) return 0;   /* out of bound – should not occur */
    return seq_vals[n - 1];
}
