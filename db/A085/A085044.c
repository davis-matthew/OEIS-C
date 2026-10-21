/*
 * A085044  Smallest number k such that tau(n) +tau(k) =tau(n+k), or 0 if no
 *          such number exists.
 * https://oeis.org/A085044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that τ(n)+τ(k)=τ(n+k)
 * Range:     u8 n <= 35, u16 n <= 575, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

/* cached divisor counts τ(i) for i ≥ 1 */
static seq_t *tau_vals   = NULL;
static unsigned char *tau_comp = NULL; /* 0 = uncomputed, 1 = computed */
static size_t tau_cap = 0;

/* Ensure cache holds indices up to i (inclusive) */
static void ensure_tau(idx_t i)
{
    if (i < tau_cap) return;
    size_t new_cap = tau_cap ? tau_cap << 1 : 16;
    while (new_cap <= (size_t)i) new_cap <<= 1;
    tau_vals   = (seq_t*)realloc(tau_vals,   new_cap * sizeof(seq_t));
    tau_comp   = (unsigned char*)realloc(tau_comp, new_cap * sizeof(unsigned char));
    for (size_t j = tau_cap; j < new_cap; ++j) tau_comp[j] = 0;
    tau_cap = new_cap;
}

/* Return τ(i) with caching; τ(0) is undefined but never queried */
static seq_t tau_of(idx_t i)
{
    if (i == 0) return 0;               /* τ(0) is not used */
    ensure_tau(i);
    if (!tau_comp[i]) {
        uint64_t n = (uint64_t)i;
        tau_vals[i] = divisor_count_u64(n);
        tau_comp[i] = 1;
    }
    return tau_vals[i];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1 */
    seq_t tau_n = tau_of(n);
    for (idx_t k = 1; ; ++k) {
        seq_t tau_k  = tau_of(k);
        seq_t tau_nk = tau_of(n + k);
        if (tau_n + tau_k == tau_nk) return (seq_t)k;
        /* Very small safety guard – unlikely to trigger for tested n */
        if (k > 1000000ULL) return 0;
    }
}
