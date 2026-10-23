/*
 * A181540  a(n) = Sum_{k=0..n} gcd(n,k)*phi(k).
 * https://oeis.org/A181540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} gcd(n,k) * phi(k)
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes φ(k) for all k ≤ n using a
   simple sieve each time a call requires a larger n. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Static cache for φ(k) values (unsigned 64‑bit).  The cache
   is grown lazily whenever a call asks for a larger n. */
static uint64_t *phi_cache = NULL;
static idx_t phi_cap = 0;

/* Ensure that phi_cache contains φ(0) … φ(n). */
static void ensure_phi(idx_t n)
{
    if (n < phi_cap) return;            /* already computed */

    idx_t new_cap = n + 1;
    uint64_t *tmp = (uint64_t *)realloc(phi_cache, new_cap * sizeof(uint64_t));
    phi_cache = tmp;

    /* Initialise φ(i) = i for all i < new_cap. */
    for (idx_t i = 0; i < new_cap; ++i)
        phi_cache[i] = i;

    /* Sieve to compute the totient function. */
    for (idx_t p = 2; p < new_cap; ++p) {
        if (phi_cache[p] == p) {          /* p is prime */
            for (idx_t j = p; j < new_cap; j += p) {
                phi_cache[j] -= phi_cache[j] / p;
            }
        }
    }
    phi_cap = new_cap;
}

/* Main function requested by the SEQOIS entry. */
seq_t a(idx_t n)
{
    /* Compute φ values up to n as needed. */
    ensure_phi(n);

    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t g = gcd((seq_t)n, (seq_t)k);          /* gcd(n,k) */
        seq_t phi_k = (seq_t)phi_cache[k];
        result += g * phi_k;
    }
    return result;
}
