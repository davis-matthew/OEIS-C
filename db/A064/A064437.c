/*
 * A064437  a(1)=1, a(n) = a(n-1) + 3 if n is already in the sequence, a(n) =
 *          a(n-1) + 2 otherwise.
 * https://oeis.org/A064437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064437:
   a(1) = 1
   a(n) = a(n-1) + 3 if n occurs as a term in {a(1), a(2), …, a(n-1)};
          a(n-1) + 2 otherwise.
*/
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static memoisation structures */
static idx_t  last_n = 0;       /* highest n computed so far */
static seq_t *terms  = NULL;    /* array of sequence terms, 0‑based index */
static size_t terms_cap = 0;    /* allocated capacity of terms */

/* presence[j] == 1 if the value j has already appeared in the sequence */
static uint8_t *presence = NULL;          /* dynamic bit array (as bytes) */
static idx_t  presence_cap = 0;          /* size in elements (max index + 1) */

/* ensure terms array has capacity at least cap */
static void ensure_terms_cap(size_t cap) {
    if (terms_cap < cap) {
        size_t new_cap = terms_cap ? terms_cap : 16;
        while (new_cap < cap) new_cap <<= 1;
        seq_t *new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) abort();
        terms = new_terms;
        terms_cap = new_cap;
    }
}

/* ensure presence array has capacity at least cap */
static void ensure_presence_cap(idx_t cap) {
    if (presence_cap < cap) {
        idx_t new_cap = presence_cap ? presence_cap : 64;
        while (new_cap < cap) new_cap <<= 1;
        uint8_t *new_pres = (uint8_t*)realloc(presence, new_cap * sizeof(uint8_t));
        if (!new_pres) abort();
        /* zero new entries */
        for (idx_t i = presence_cap; i < new_cap; ++i) new_pres[i] = 0;
        presence = new_pres;
        presence_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but safe */

    /* if product already computed, just return */
    if (n <= last_n) {
        return terms[n-1];
    }

    /* initialise structures if first call */
    if (terms == NULL) {
        ensure_terms_cap(n+1);
        ensure_presence_cap(128);  /* initial capacity for presence */
        terms[0] = (seq_t)1;               /* a(1) = 1 */
        ensure_presence_cap(2);            /* ensure index 1 exists */
        presence[1] = 1;                   /* 1 is present */
        last_n = 1;
    }

    seq_t last_val = terms[last_n-1];

    for (idx_t i = last_n + 1; i <= n; ++i) {
        seq_t step;
        if (i == 1) {
            /* handled above, skip but keep for completeness */
            step = 0;
        } else {
            /* check if index i already appears as a term value */
            bool in_seq = (i < presence_cap && presence[i] != 0);
            step = in_seq ? (seq_t)3 : (seq_t)2;
        }
        seq_t new_val = last_val + step;
        ensure_terms_cap(last_n + 1);
        terms[last_n] = new_val;

        /* mark presence of the new value */
        if ((idx_t)new_val < presence_cap) {
            presence[(idx_t)new_val] = 1;
        } else {
            /* grow presence array just enough to include the new value */
            ensure_presence_cap((idx_t)new_val + 1);
            presence[(idx_t)new_val] = 1;
        }

        last_val = new_val;
        ++last_n;
    }

    return terms[n-1];
}
