/*
 * A100867  Smallest positive integer k such that A000037(n) is not a
 *          quadratic residue modulo k.
 * https://oeis.org/A100867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_quadratic_residue_mod_k, isqrt_u64, modpow_seq, qr_mod_2_pow,
 *            qr_mod_prime_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100867:
   a(n) = smallest k >= 2 such that A000037(n)
   (the n-th positive nonsquare integer) is not a quadratic residue modulo k
   formula: a(n) = A139401(A000037(n)) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"   /* idx_t isqrt_u64(idx_t) */
#include "../utils/is_quadratic_residue_mod_k.h"
#include "../utils/modpow_seq.h"
#include "../utils/qr_mod_2_pow.h"
#include "../utils/qr_mod_prime_pow.h"

/* return the n-th non‑square integer, i.e., A000037(n) */
static inline seq_t nth_non_square(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n)
    {
        ++m;
        /* test for perfect square using isqrt_u64 */
        idx_t r = isqrt_u64(m);
        if (r * r != m)
            ++count;
    }
    return (seq_t)m;
}

/* sequence a(n) */
seq_t a(idx_t n)
{
    seq_t m = nth_non_square(n);       /* A000037(n) */
    for (idx_t k = 2; ; ++k)
    {
        if (!is_quadratic_residue_mod_k(m, k))
            return (seq_t)k;          /* minimal k with non‑residue property */
    }
}
