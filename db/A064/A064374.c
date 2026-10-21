/*
 * A064374  Numbers k such that sigma(k) > phi(k)^2.
 * https://oeis.org/A064374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   calc_phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064374: Numbers k such that sigma(k) > phi(k)^2.
   Computation uses the helper functions sigma_of and calc_phi
   (included from "../utils/sigma_of.h" and "../utils/calc_phi.h").
   The set of such numbers is finite (2,4,6,10,12,18,30).  The
   implementation dynamically generates all terms once and
   returns the n‑th term (offset 1).  For n larger than the available
   terms it returns 0. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/calc_phi.h"

static seq_t *terms = NULL;     /* array of found terms */
static size_t terms_len = 0;    /* number of terms stored */

/* Generate all terms at once (once) */
static void init_terms(void)
{
    if (terms != NULL) return;      /* already initialized */

    /* This sequence has only 7 terms.  We scan up to 1000 to be safe. */
    const idx_t LIMIT = 1000;

    terms_len = 0;
    size_t cap = 8;                  /* initial capacity */
    terms = (seq_t *)malloc(cap * sizeof(seq_t));
    if (terms == NULL) return;       /* allocation failed; leave empty */    

    for (idx_t k = 2; k <= LIMIT; ++k) {
        seq_t sigma_k = sigma_of(k);
        seq_t phi_k   = calc_phi(k);

        /* phi_k * phi_k fits in seq_t for k <= LIMIT */
        if (sigma_k > phi_k * phi_k) {
            if (terms_len >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (tmp == NULL) { free(terms); terms = NULL; return; }
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
}

/* External function required by the framework */
seq_t a(idx_t n)
{
    init_terms();                  /* ensure terms are ready */

    if (n == 0 || terms == NULL) return 0;   /* invalid index or failure */

    if (n <= terms_len) return terms[n - 1];
    return 0; /* no further terms */
}
