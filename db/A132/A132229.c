/*
 * A132229  The set N such that each positive integer can be written in the
 *          form s^2 + n, s>=0, n in N, in an odd number of ways.
 * https://oeis.org/A132229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A132229: exponents of 1/((1‑x)S) modulo 2
   where S = ∑_{s≥0} x^{s²}.  The function returns the n‑th exponent
   (1‑based). */
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system.  Avoid using other
   utilities or floating point.  All arithmetic is integer. */

static uint8_t *coeff = NULL;      /* coeff[e] = g[e] (0 or 1) */
static size_t coeff_size  = 0;     /* number of computed coeff entries */

static seq_t *terms = NULL;        /* list of exponents where coeff[e]==1 */
static size_t terms_size = 0;
static size_t terms_cap  = 0;

/* Build coeff[0..] and terms until at least n exponents are known. */
static void ensure_terms(idx_t n)
{
    if (terms_size >= n)
        return;                     /* already have enough terms */

    /* Initialise the coefficient array if not yet done. */
    if (coeff_size == 0) {
        coeff_size = 1;
        coeff = (uint8_t *)malloc(coeff_size * sizeof(uint8_t));
        coeff[0] = 1;                /* H(x) has constant term 1 */
    }

    idx_t e = coeff_size;           /* next exponent to compute */

    while (terms_size < n) {
        /* Ensure coeff array can hold coefficient e. */
        if ((size_t)e >= coeff_size) {
            coeff_size = e + 1;
            coeff = (uint8_t *)realloc(coeff, coeff_size * sizeof(uint8_t));
        }

        uint8_t val = 0;

        /* Contributions from k = s² (s=2,3,…).  k=1 (s=1) is omitted because
           h[1] = 0. */
        for (idx_t s = 1; s * s <= e; ++s) {
            if (s == 1)
                continue;           /* skip k = 1 */
            val ^= coeff[e - s * s];
        }

        /* Contributions from k = s²+1 (s=1,2,…).  s=0 would give k=1,
           which is omitted. */
        for (idx_t s = 1; (s * s + 1) <= e; ++s) {
            val ^= coeff[e - (s * s + 1)];
        }

        coeff[e] = val;

        if (val) {                  /* exponent e belongs to the sequence */
            if (terms_size == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 4;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_size++] = (seq_t)e;
        }

        ++e;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined, return 0 for safety */
    ensure_terms(n);
    return terms[n - 1];
}
