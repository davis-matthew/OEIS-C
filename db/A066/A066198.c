/*
 * A066198  Numbers k where phi changes as fast as sigma, i.e., abs(phi(k+1) -
 *          phi(k)) = abs(sigma(k+1) - sigma(k)).
 * https://oeis.org/A066198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 384, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer k such that |phi(k+1)-phi(k)| == |sigma(k+1)-sigma(k)| */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *found = NULL;
    static size_t sz = 0;       /* number of terms stored */
    static size_t cap = 0;      /* capacity of the array */
    static idx_t last_k = 0;    /* last k processed */
    static seq_t phi_prev = 0, sigma_prev = 0;
    static bool init = false;

    if (!init) {
        init = true;
        cap = 64;
        found = (seq_t *)malloc(cap * sizeof(seq_t));
        last_k = 1;                     /* start from k = 1 */
        phi_prev   = phi(last_k);       /* φ(1) = 1 */
        sigma_prev = sigma_of(last_k);  /* σ(1) = 1 */
    }

    /* generate terms until we have at least n terms */
    while (sz < n) {
        idx_t      k_next   = last_k + 1;
        seq_t      phi_next = phi(k_next);
        seq_t      sigma_next = sigma_of(k_next);

        seq_t diff_phi   = phi_next > phi_prev ? phi_next - phi_prev : phi_prev - phi_next;
        seq_t diff_sigma = sigma_next > sigma_prev ? sigma_next - sigma_prev : sigma_prev - sigma_next;

        if (diff_phi == diff_sigma) {
            if (sz == cap) {
                cap *= 2;
                found = (seq_t *)realloc(found, cap * sizeof(seq_t));
            }
            found[sz++] = (seq_t)last_k;    /* store k where the relation holds */
        }

        /* advance to next k */
        phi_prev   = phi_next;
        sigma_prev = sigma_next;
        last_k     = k_next;
    }

    return found[n - 1];
}
