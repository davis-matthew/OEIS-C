/*
 * A223908  Poly-Cauchy numbers of the second kind -hat c_5^(-n).
 * https://oeis.org/A223908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{m=1}^{5} c_m · (m+1)^n with coefficients c = {24, 50,
 *            35, 10, 1}
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation by squaring for seq_t.  This works both for built‑in
   integer types (using the usual arithmetic operators) and for the
   arbitrary‑precision seq_t class (which overloads these operators). */
static seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* safety for n < 1 */

    seq_t p2 = power_seq((seq_t)2, n);
    seq_t p3 = power_seq((seq_t)3, n);
    seq_t p4 = power_seq((seq_t)4, n);
    seq_t p5 = power_seq((seq_t)5, n);
    seq_t p6 = power_seq((seq_t)6, n);

    seq_t sum = 0;
    sum += p2 * (seq_t)24;   /* 2^n * 24      */
    sum += p3 * (seq_t)50;   /* 3^n * 50      */
    sum += p4 * (seq_t)35;   /* 4^n * 35      */
    sum += p5 * (seq_t)10;   /* 5^n * 10      */
    sum += p6;               /* 6^n * 1       */

    return sum;
}
