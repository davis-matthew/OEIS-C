/*
 * A095222  (Concatenation of T(n)+1..T(n+1)) mod (concatenation of
 *          T(n-1)+1..T(n)), where T(k) is the k-th triangular number,
 *          A000217(k).
 * https://oeis.org/A095222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   a(n) = (concatenation_{i=T(n)+1}^{T(n+1)} i) mod
 *            (concatenation_{i=T(n-1)+1}^{T(n)} i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where T(k) = k*(k+1)/2 is the k‑th triangular number. */

#include "../utils/pow10.h"
#include "../utils/digits_of.h"

/* Compute the A095222 term for index n (n >= 1). */
seq_t a(idx_t n)
{
    /* Convert n to the required seq_t form for triangular numbers. */
    seq_t sn = (seq_t)n;
    /* Triangular numbers T(n-1), T(n), T(n+1) as idx_t values (safe for n <= ~10^9). */
    idx_t Tn_minus1 = (idx_t)((seq_t)(n-1) * ((seq_t)(n-1) + 1) / 2);
    idx_t Tn        = (idx_t)(sn * (sn + 1) / 2);
    idx_t Tn_plus1  = (idx_t)((seq_t)(n+1) * ((seq_t)(n+1) + 1) / 2);

    /* Build the denominator D = concatenation of T(n-1)+1 .. T(n). */
    seq_t D = 0;
    for (idx_t i = Tn_minus1 + 1; i <= Tn; ++i) {
        idx_t d    = digits_of(i);               /* number of decimal digits of i */
        seq_t mult = pow10((idx_t)d);            /* 10^d as seq_t */
        D = D * mult + i;                        /* D = D*10^d + i */
    }

    /* Compute N mod D, where N is the concatenation T(n)+1 .. T(n+1). */
    seq_t rem = 0;
    for (idx_t i = Tn + 1; i <= Tn_plus1; ++i) {
        idx_t d    = digits_of(i);
        seq_t mult = pow10((idx_t)d);
        rem = (rem * mult + i) % D;
    }

    return rem;
}
