/*
 * A138591  Sums of two or more consecutive nonnegative integers.
 * https://oeis.org/A138591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + A000523(n + A000523(n)) Equivalently: for n > 1, let
 *            m = ⌊log₂(n‑1)⌋ + 1. Then a(n) = n + m if (n + m) > 2ᵐ = n + m –
 *            1 otherwise. a(1) = 1
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers except powers of two
   (with 1 included because 1 = 0 + 1). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 1;                     /* base case */

    idx_t nm1 = n - 1;                        /* n > 1 here */
    /* floor(log₂(nm1)) using a built‑in helper */
    unsigned floor_log = 63 - __builtin_clzll((unsigned long long)nm1);
    unsigned m = floor_log + 1;               /* m = ⌊log₂(n‑1)⌋ + 1 */

    seq_t pow2 = ((seq_t)1) << m;             /* 2ᵐ  (seq_t‑wide shift) */
    seq_t candidate = (seq_t)n + (seq_t)m;     /* n + m  */

    return (candidate > pow2) ? candidate : candidate - 1;
}
