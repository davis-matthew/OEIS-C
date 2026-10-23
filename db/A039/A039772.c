/*
 * A039772  Even numbers k such that phi(k) and k-1 are distinct and have a
 *          common factor > 1.
 * https://oeis.org/A039772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 9743, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the terms of A039772.
 * It simply scans even integers k and keeps those for which
 * phi(k) ≠ k-1 and gcd(phi(k),k-1) > 1.
 * The sequence is built incrementally and cached to allow
 * out-of-order queries. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"

static seq_t *terms = NULL;      /* cached terms */
static size_t terms_cnt = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of allocated array */
static uint64_t last_checked = 2;/* last even integer examined */

/* Ensure we have room for `need` terms. */
static void
ensure_capacity(size_t need)
{
    if (need <= terms_cap)
        return;

    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n = 0 */

    /* generate terms until we have at least n of them */
    while (terms_cnt < (size_t)n) {
        last_checked += 2;          /* next even integer */
        seq_t k  = (seq_t)last_checked;
        seq_t phi_k = phi((idx_t)k);
        seq_t km1 = k - 1;

        if (phi_k != km1) {
            seq_t g = gcd(phi_k, km1);
            if (g > (seq_t)1) {
                ensure_capacity(terms_cnt + 1);
                terms[terms_cnt++] = k;
            }
        }
    }

    return terms[(size_t)n - 1];
}
