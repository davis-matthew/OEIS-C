/*
 * A190503  Numbers k such that sigma(phi(k)) divides sigma(k).
 * https://oeis.org/A190503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 470, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term a(n) of OEIS sequence A190503:
 * numbers k such that σ(φ(k)) divides σ(k).
 * The function scans successive integers k and stores those that
 * satisfy the divisibility.  Results are memoised so that any call
 * with a larger or arbitrary index is answered in O(1) after the
 * required terms have been generated. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

static seq_t *terms = NULL;      /* stored terms */
static size_t terms_len = 0;     /* number of stored terms */
static size_t terms_cap = 0;     /* allocated capacity of the array */
static idx_t last_checked = 0;   /* last integer k that has been examined */

/* Ensure that at least `needed` terms are available. */
static void generate_to(idx_t needed)
{
    while (terms_len < needed)
    {
        ++last_checked;                                 /* next k to test */
        idx_t k = last_checked;

        seq_t sigma_k   = sigma_of(k);                  /* σ(k) */
        seq_t phi_k     = phi(k);                       /* φ(k)   */
        idx_t phi_val   = (idx_t)phi_k;                 /* φ(k) as idx_t for sigma_of */
        seq_t sigma_phi = sigma_of(phi_val);            /* σ(φ(k)) */

        /* test whether σ(φ(k)) | σ(k) */
        if (sigma_phi != (seq_t)0 && (sigma_k % sigma_phi) == (seq_t)0)
        {
            if (terms_len == terms_cap)               /* enlarge array if needed */
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();                    /* fatal error */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)k;
        }
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined for offset 1 */
    if (terms_len < n) generate_to(n);
    return terms[n - 1];
}
