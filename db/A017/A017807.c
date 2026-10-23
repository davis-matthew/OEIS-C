/*
 * A017807  Binomial coefficients C(91,n).
 * https://oeis.org/A017807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binom(91,n) (for 0 ≤ n ≤ 91; a(n)=0 for n>91)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 19, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t, etc. */

/* Compute C(91,n) using a multiplicative algorithm that avoids
   intermediate overflow by using 128‑bit arithmetic when compiling
   as plain C and relying on big‑integer operators when compiling
   as C++ (seq_t may be an arbitrary‑precision class). */
seq_t a(idx_t n)
{
    /* Binomial is zero outside the range 0 .. 91 */
    if (n > 91) {
        return 0;
    }

    /* Use symmetry to minimize the number of iterations */
    idx_t k = (n > 91 - n) ? 91 - n : n;   /* 0 ≤ k ≤ 91/2 */

    seq_t res = 1;

    /* Multiplicative formula:
       res = 1;
       for i = 1 .. k:
           res = res * (91 - k + i) / i;
       The division is always exact. */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(91 - k + i);
        seq_t den = (seq_t)i;

#ifdef __cplusplus
        /* In C++ seq_t may be a big‑integer class with operator* / */
        res = (res * num) / den;
#else
        /* In C we use an unsigned 128‑bit type to avoid overflow.
           This is a GCC/Clang extension but available in the
           environments expected for this project. */
        res = (seq_t)(((__int128)res * num) / den);
#endif
    }

    return res;
}
