/*
 * A226117  Numbers k such that phi(sigma(tau(k))) = tau(sigma(phi(k))).
 * https://oeis.org/A226117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 468, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, calc_phi, divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A226117 by brute‑force search.  */
/* condition: calc_phi( sigma_of( divisor_count_u64(k) ) ) ==
   divisor_count_u64( sigma_of( calc_phi(k) ) ) */

#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/calc_phi.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Cache of discovered terms */
    static seq_t *terms   = NULL;
    static size_t   len   = 0;
    static size_t   cap   = 0;
    static idx_t    next_k = 1;   /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (len < (size_t)n) {
        seq_t k = (seq_t)next_k;          /* current candidate */

        /* φ(k) */
        seq_t phi_k = calc_phi(k);

        /* τ(k) */
        idx_t tau_k = divisor_count_u64((uint64_t)next_k);

        /* σ(τ(k)) and φ(σ(τ(k))) */
        seq_t sigma_tau_k = sigma_of(tau_k);
        seq_t lhs = calc_phi(sigma_tau_k);

        /* σ(φ(k)) and τ(σ(φ(k))) */
        seq_t sigma_phi_k = sigma_of((idx_t)phi_k);
        idx_t rhs = divisor_count_u64((uint64_t)sigma_phi_k);

        /* If the equality holds, store the term */
        if (lhs == (seq_t)rhs)
            append_candidate(&terms, &len, &cap, k);

        ++next_k;
    }

    return terms[n-1];
}
