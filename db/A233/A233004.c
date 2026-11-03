/*
 * A233004  Pt(n) mod n!, where Pt(n) is product of first n positive
 *          triangular numbers (A000217).
 * https://oeis.org/A233004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{k=1}^n T_k) mod n!, where T_k = k(k+1)/2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but safeguard */

    seq_t mod = factorial(n);            /* n! */
    seq_t prod = 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t t = ((seq_t)k * (k + 1)) / 2;          /* T_k is integer */
        prod = (unsigned __int128)prod * t % mod;    /* long multiplication */
    }
    return prod;
}
