/*
 * A076951  Smallest k > 0 such that nk-1 is an n-th power, or 0 if no such
 *          number exists.
 * https://oeis.org/A076951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that n*k-1 is an n‑th power, or 0 if
 *            no such k exists. The algorithm searches for the smallest m in
 *            [1,n-1] with m^n ≡ -1 (mod n), then computes k = (m^n+1)/n. For
 *            built‑in integer seq_t a 128‑bit intermediate is used; if the
 *            result does not fit into seq_t the function...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 18, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow, compute_k_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/big_pow.h"
#include "../utils/compute_k_small.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    /* Search for the smallest m with m^n ≡ -1 (mod n) */
    for (idx_t m = 1; m < n; ++m) {
        if (mod_pow((seq_t)m, (uint64_t)n, (seq_t)n) == (seq_t)n - 1) {
            /* Found a suitable m; compute the corresponding k */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                /* Built‑in integer type: use 128‑bit intermediate */
                return compute_k_small(n, (seq_t)m);
            } else {
                /* Arbitrary‑precision seq_t: use big_pow */
                seq_t mpow = big_pow((seq_t)m, (seq_t)n);
                seq_t k    = (mpow + 1) / (seq_t)n;
                return k;
            }
        }
    }
    /* No solution found */
    return (seq_t)0;
}
