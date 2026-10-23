/*
 * A108345  The set N such that each positive integer can be written in the
 *          form s^2 + n, s>=0, n in N, in an even number of ways.
 * https://oeis.org/A108345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k ≥ 0 such that the parity of *
 *            representations of k as s² + n (s ≥ 0, n ∈ N) is odd. * * Let a₀
 *            = 1. For k ≥ 1: * a_k = ⊕_{s≥1, s² ≤ k} a_{k–s²} (⊕ = XOR = +
 *            mod 2). * * The set of k with a_k = 1 is the sequence; a(n) is
 *            the n‑th such k * (offset 1). The implementation me...
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_bit_cap, ensure_term_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ensure_bit_cap.h"
#include "../utils/ensure_term_cap.h"

seq_t a(idx_t n)
{
    /* Persistent state across calls. */
    static char *bit = NULL;          /* parity values a_k: 0 or 1 */
    static idx_t bit_cap = 0, bit_len = 0;
    static seq_t *terms = NULL;       /* list of k with a_k == 1 */
    static idx_t terms_cap = 0, terms_len = 0;

    /* Initialise the first parity value a₀ = 1 and store 0 as the first term. */
    if (bit_len == 0) {
        ensure_bit_cap(&bit, &bit_cap, 0);
        bit[0] = 1;
        bit_len = 1;

        ensure_term_cap(&terms, &terms_cap, 0);
        terms[0] = (seq_t)0;   /* k = 0 is in the set */
        terms_len = 1;
    }

    /* Generate values until we have at least n terms. */
    while (terms_len < n) {
        idx_t k = bit_len;          /* Next index to compute */
        unsigned char val = 0;      /* Parity accumulator */

        /* Compute a_k = XOR of a_{k – s²} for s ≥ 1, s² ≤ k. */
        for (idx_t s = 1; s * s <= k; ++s)
            val ^= bit[k - s * s];

        ensure_bit_cap(&bit, &bit_cap, k);
        bit[k] = val;
        ++bit_len;

        if (val) {                  /* a_k == 1 → add k to the sequence */
            ensure_term_cap(&terms, &terms_cap, terms_len);
            terms[terms_len++] = (seq_t)k;
        }
    }

    return terms[n - 1];
}
