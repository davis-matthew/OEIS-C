/*
 * A162791  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=16, a(2)=240, a(3)=3480 Recurrence for n>=4: a(n) =
 *            14 a(n-1) + 14 a(n-2) - 105 a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the rational generating function 
   A(t) = (t^3 + 2 t^2 + 2 t + 1) / (1 - 14 t - 14 t^2 + 105 t^3)     */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)240;
    if (n == 3) return (seq_t)3480;

    seq_t a0 = (seq_t)1;      /* a(n-3) */
    seq_t a1 = (seq_t)16;     /* a(n-2) */
    seq_t a2 = (seq_t)240;    /* a(n-1) */
    seq_t a3 = (seq_t)3480;   /* a(n)   */

    /* Iterate from 4 up to n using the recurrence. */
    for (idx_t k = 4; k <= n; ++k) {
        seq_t next = (seq_t)14 * a3 + (seq_t)14 * a2 - (seq_t)105 * a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
