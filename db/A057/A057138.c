/*
 * A057138  Add (n mod 10)*10^(n-1) to the previous term, with a(0) = 0.
 * https://oeis.org/A057138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057138: a(0) = 0, a(n) = a(n‑1) + (n mod 10) * 10^(n‑1)  */
/* Recurrence: a(n) = a(n‑1) + ((n % 10) * 10^(n‑1)),   a(0)=0          */

#include <stddef.h>   /* for size_t if needed, not used directly */

/* Computes the n‑th term of A057138. */
seq_t a(idx_t n)
{
    seq_t a = (seq_t)0;          /* current term */
    seq_t pow = (seq_t)1;        /* pow = 10^(i-1) for the loop iteration i */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t add = pow * (seq_t)(i % 10u);   /* (n mod 10) * 10^(n-1) */
        a += add;
        pow *= (seq_t)10;                     /* prepare 10^i for next i */
    }

    return a;
}
