/*
 * A083745  a(1) = 1; if a(n-1) + n is composite or 1 then a(n) = a(n-1) + n,
 *          else a(n) = a(n-1) - n.
 * https://oeis.org/A083745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 297, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083745
   a(1) = 1
   for n>1, let s = a(n‑1) + n
   if s is composite or 1 then a(n) = s
   else a(n) = a(n‑1) – n
   (composite is defined to include ≤1, 0 or negative numbers)
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1, and behaviour for n<1 is irrelevant */
        return (seq_t)1;

    seq_t prev = (seq_t)1;    /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cand = prev + (seq_t)i;

        bool composite;
        if (cand <= (seq_t)1)                   /* 0, 1 or negative */
            composite = true;
        else
            composite = !is_prime((uint64_t)cand);

        prev = composite ? cand : prev - (seq_t)i;
    }
    return prev;
}
