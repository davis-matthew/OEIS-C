/*
 * A226117  Numbers k such that phi(sigma(tau(k))) = tau(sigma(phi(k))).
 * https://oeis.org/A226117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 468, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 786 (2026-11-03)
 * Helpers:   mem, num_divisors, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A226117 by brute‑force search.  */
/* condition: calc_phi( sigma_of( divisor_count_u64(k) ) ) ==
   divisor_count_u64( sigma_of( calc_phi(k) ) ) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/totient_seq.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
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
    /* Cache of discovered terms */
    static seq_t *terms   = NULL;
    static size_t   len   = 0;
    static size_t   cap   = 0;
    static idx_t    next_k = 1;   /* next integer to test */

    /* Generate terms until we have at least n of them */
    while (len < (size_t)n) {
        seq_t k = (seq_t)next_k;          /* current candidate */

        /* φ(k) */
        seq_t phi_k = totient_seq(k);

        /* τ(k) */
        idx_t tau_k = num_divisors((uint64_t)next_k);

        /* σ(τ(k)) and φ(σ(τ(k))) */
        seq_t sigma_tau_k = sigma_of(tau_k);
        seq_t lhs = totient_seq(sigma_tau_k);

        /* σ(φ(k)) and τ(σ(φ(k))) */
        seq_t sigma_phi_k = sigma_of((idx_t)phi_k);
        idx_t rhs = num_divisors((uint64_t)sigma_phi_k);

        /* If the equality holds, store the term */
        if (lhs == (seq_t)rhs)
            append_candidate(&terms, &len, &cap, k);

        ++next_k;
    }

    return terms[n-1];
}
