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
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   ipow, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/powmod.h"

/* Helper that computes (m^n + 1) / n for small seq_t types
   using a 128‑bit intermediate.  It returns 0 if an overflow
   that would overflow seq_t occurs. */
static seq_t compute_k_small(idx_t n, seq_t m)
{
    unsigned __int128 res = 1;
    unsigned __int128 um  = (unsigned __int128)m;
    for (idx_t i = 0; i < n; ++i) {
        if (res > ((unsigned __int128)-1) / um) /* would overflow 128 bits */
            return (seq_t)0;
        res *= um;
    }
    unsigned __int128 k128 = (res + 1) / n;

    /* check if k fits in seq_t */
    seq_t max_seq = (seq_t)-1;  /* maximum value of seq_t */
    if (k128 > (unsigned __int128)max_seq)
        return (seq_t)0;
    return (seq_t)k128;
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    /* Search for the smallest m with m^n ≡ -1 (mod n) */
    for (idx_t m = 1; m < n; ++m) {
        if (powmod((seq_t)m, (uint64_t)n, (seq_t)n) == (seq_t)n - 1) {
            /* Found a suitable m; compute the corresponding k */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                /* Built‑in integer type: use 128‑bit intermediate */
                return compute_k_small(n, (seq_t)m);
            } else {
                /* Arbitrary‑precision seq_t: use big_pow */
                seq_t mpow = ipow((seq_t)m, (seq_t)n);
                seq_t k    = (mpow + 1) / (seq_t)n;
                return k;
            }
        }
    }
    /* No solution found */
    return (seq_t)0;
}
