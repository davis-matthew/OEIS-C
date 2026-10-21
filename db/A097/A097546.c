/*
 * A097546  Denominators of "Farey fraction" approximations to Pi.
 * https://oeis.org/A097546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Approximations of π via the Stern–Brocot mediant sequence.
   a(0)=0 (denominator of 1/0), a(1)=1 (denominator of 0/1),
   for n≥2 a(n) is the denominator of the nth mediant inserted
   while repeatedly bracketing π with left and right extremes. */
#include <stdint.h>

static const unsigned long long SCALE   = 1000000000000000ULL;   /* 10^15 */
static const unsigned long long PI_INT  = 3141592653589793ULL;   /* π ≈ 3.141592653589793 */

/* returns the nth term of the sequence */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* denominator of 1/0 */
    if (n == 1) return (seq_t)1;   /* denominator of 0/1 */

    /* left fraction = 0/1, right fraction = 1/0 */
    seq_t left_n   = (seq_t)0, left_d   = (seq_t)1;
    seq_t right_n  = (seq_t)1, right_d  = (seq_t)0;
    idx_t k = 2;                       /* current index */

    while (1) {
        /* mediant fraction */
        seq_t mediant_n = left_n + right_n;
        seq_t mediant_d = left_d + right_d;

        /* compare mediant_n/mediant_d with π using high‑precision integer math */
        unsigned __int128 lhs = (unsigned __int128)mediant_n * SCALE;
        unsigned __int128 rhs = (unsigned __int128)mediant_d * PI_INT;

        if (lhs < rhs) {             /* mediant < π → update left side */
            left_n = mediant_n;
            left_d = mediant_d;
        } else {                     /* mediant ≥ π → update right side */
            right_n = mediant_n;
            right_d = mediant_d;
        }

        if (k == n) return mediant_d;   /* desired term reached */
        ++k;
    }
}
