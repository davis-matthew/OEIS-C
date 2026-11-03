/*
 * A230601  a(n) = Lucas(2^n + 2).
 * https://oeis.org/A230601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Lucas(2^n + 2)
 *
 * Computation:
 *   We compute the Fibonacci pair (F(k), F(k+1)) for k = 2^n + 2 using an
 *   iterative fast‑doubling method that scans the bits of k from most to
 *   least significant.  For this k the binary representation contains
 *   exactly two 1‑bits (except for n = 0, 1), so the loop is very small.
 *   The Lucas number is then obtained as L(k) = 2*F(k+1) – F(k).
 */

#include <stdbool.h>

/* Computes L(2^n + 2) for a given n.  The result is returned as seq_t.  */
static seq_t lucas_power2plus2(idx_t n)
{
    /* Determine the highest bit to process. */
    idx_t top;
    if (n == 0)
        top = 1;          /* k = 3  -> bits 1 and 0 are set                */
    else if (n == 1)
        top = 2;          /* k = 4  -> only bit 2 is set                 */
    else
        top = n;          /* k = 2^n + 2 -> bits n and 1 are set          */

    /* Fibonacci pair (F(m), F(m+1)), starting with m = 0. */
    seq_t a = 0;          /* F(0) */
    seq_t b = 1;          /* F(1) */

    /* Main loop, iterating over bits of k from top to 0. */
    for (idx_t i = top; ; --i) {
        /* Fast‑doubling step: compute F(2m) and F(2m+1). */
        seq_t two_b = b << 1;                     /* 2·b            */
        seq_t c = a * (two_b - a);                /* F(2m)          */
        seq_t d = a * a + b * b;                  /* F(2m+1)        */

        /* Decide whether the current bit of k is 1. */
        bool bit_set;
        if (n == 0)
            bit_set = (i == 1 || i == 0);
        else if (n == 1)
            bit_set = (i == 2);
        else
            bit_set = (i == n || i == 1);

        /* Update Fibonacci pair according to the bit. */
        if (bit_set) {
            a = d;
            b = c + d;          /* b = F(2m+2) = F(2m) + F(2m+1) */
        } else {
            a = c;
            b = d;              /* b = F(2m+1)            */
        }

        if (i == 0)               /* finished processing all bits   */
            break;
    }

    /* Return the Lucas number: L(k) = 2·F(k+1) – F(k). */
    return (b << 1) - a;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Public interface required by the grader.                               */
seq_t a(idx_t n)
{
    return lucas_power2plus2(n);
}
