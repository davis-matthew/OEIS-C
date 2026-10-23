/*
 * A117303  Self-inverse permutation of the natural numbers based on the
 *          bijection (2*x-1)*2^(y-1) <--> (2*y-1)*2^(x-1).
 * https://oeis.org/A117303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117303: Self‑inverse permutation of ℕ.
 * Formula: let n = (2*x‑1)·2^(y‑1) with (2*x‑1) odd.
 * Then a(n) = (2*y‑1)·2^(x‑1).
 * Equivalently, if k = v₂(n) (exponent of 2) and odd = n >> k,
 * x = (odd+1)/2, then a(n) = (2*k+1) · 2^(x‑1).  */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0; return 0 for safety */
        return (seq_t)0;

    /* compute exponent of 2 in n (v₂(n)) */
    idx_t tmp = n;
    idx_t k   = 0;
    while ((tmp & 1ULL) == 0ULL) {
        tmp >>= 1;
        ++k;
    }

    /* odd part of n */
    seq_t odd = (seq_t)(n >> k);

    /* x = (odd + 1)/2  (odd is always odd, so division is exact) */
    seq_t x = (odd + 1) / 2;

    /* (2*k + 1) part */
    seq_t factor = ((seq_t)2 * k) + 1;

    /* exponent for pow2, which must be an idx_t */
    idx_t exp = (idx_t)(x - 1);

    return factor * pow2(exp);
}
