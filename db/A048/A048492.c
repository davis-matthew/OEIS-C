/*
 * A048492  a(n) = (8*(2^n) - n^2 - 3*n - 6)/2.
 * https://oeis.org/A048492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A048492
   Formula (OEIS): a(n) = (8*2^n - n^2 - 3*n - 6)/2
   Recurrence (derived): a(n) = 5*a(n-1) - 9*a(n-2) + 7*a(n-3) - 2*a(n-4), n>3.
   All calculations use unsigned arithmetic; overflow in intermediate
   steps is intentional and correct for the 64‑bit build.
*/

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t two_pow = pow2(n);                /* 2^n */
    seq_t num = two_pow * (seq_t)4;         /* 4*2^n  (=2^(n+2)), wraps on 64‑bit */
    seq_t sub = (n_seq * n_seq + n_seq * (seq_t)3 + (seq_t)6) / (seq_t)2;
    return num - sub;                       /* unsigned subtraction wraps as needed */
}
