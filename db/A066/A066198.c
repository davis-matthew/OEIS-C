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
 * Validated: standard, pass, 66 terms through n = 968 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th integer k such that |phi(k+1)-phi(k)| == |sigma(k+1)-sigma(k)| */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t d = 2;
    for (; d * d <= m; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            result -= result / (seq_t)d;
        }
    }
    if (m > 1) result -= result / (seq_t)m;
    return result;
}

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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
        found = mem_new(seq_t, cap);
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
                found = mem_resize(seq_t, found, cap);
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
