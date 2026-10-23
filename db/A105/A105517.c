/*
 * A105517  Number of times 7 is the leading digit of the first n+1 Fibonacci
 *          numbers in decimal representation.
 * https://oeis.org/A105517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k : 0 ≤ k ≤ n, first decimal digit of F_k = 7}
 * Range:     u8 n <= 4407, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   leading_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses Fibonacci leading‑digit via Binet approximation for k > 93;   */
/* precomputes exact small Fibonacci leading digits up to k = 93.    */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/leading_digit_uint64.h"

/* log10(φ) and log10(√5) for Binet formula */
static const long double LOG10PHI   = log10l((sqrtl(5.0L) + 1.0L) / 2.0L);
static const long double LOG10SQR5 = log10l(sqrtl(5.0L));

/* leading digits for Fibonacci numbers 0 … 93 (inclusive) */
static int leading_small[94];
static bool small_init = false;

/* initialise leading_small[0..93] once */
static void init_small_leading(void)
{
    if (small_init) return;

    unsigned __int128 a = 0, b = 1;
    leading_small[0] = leading_digit_uint64((uint64_t)a); /* F0 = 0 */
    leading_small[1] = leading_digit_uint64((uint64_t)b); /* F1 = 1 */

    for (idx_t i = 2; i <= 93; ++i) {
        unsigned __int128 c = a + b; /* next Fibonacci number */
        a = b;
        b = c;
        leading_small[i] = leading_digit_uint64((uint64_t)b); /* store F_i */
    }
    small_init = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_small_leading();

    seq_t count = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        int d;
        if (k <= 93) {
            d = leading_small[(int)k];
        } else {
            long double t = (long double)k * LOG10PHI - LOG10SQR5;
            t -= floorl(t);                                 /* fractional part */
            long double val = powl(10.0L, t) + 1e-12L;      /* tiny epsilon */
            d = (int)val;                                  /* floor gives the leading digit */
        }
        if (d == 7) count += (seq_t)1;
    }
    return count;
}
