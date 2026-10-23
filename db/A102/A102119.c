/*
 * A102119  Iccanobirt numbers (9 of 15): a(n) = R(a(n-1) + a(n-2) +
 *          R(a(n-3))), where R is the digit reversal function A004086.
 * https://oeis.org/A102119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R( a(n‑1) + a(n‑2) + R( a(n‑3) ) ) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   reverse_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_seq.h"
/* Turn a seq_t integer into its decimal digit reversal */

/* ==================================================================== */
seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Keep only the last three values needed for the recurrence. */
    seq_t a_minus3 = (seq_t)0; /* a(k-3) */
    seq_t a_minus2 = (seq_t)0; /* a(k-2) */
    seq_t a_minus1 = (seq_t)1; /* a(k-1) */
    seq_t current;

    /* Compute a(3) .. a(n).  Since n is unsigned we use idx_t. */
    for (idx_t k = 3; k <= n; ++k) {
        seq_t rev_a_m3 = reverse_seq(a_minus3);
        seq_t sum = rev_a_m3 + a_minus2 + a_minus1;
        current = reverse_seq(sum);

        /* Slide the window. */
        a_minus3 = a_minus2;
        a_minus2 = a_minus1;
        a_minus1 = current;
    }
    return a_minus1;
}
