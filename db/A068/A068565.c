/*
 * A068565  Denominators of Sum_{k=1..n} 1/(k * 2^k).
 * https://oeis.org/A068565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of ∑_{k=1}^n 1/(k·2^k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>      /* for bool type if needed */

/* Euclidean algorithm for seq_t – works for built‑in integers
   and arbitrary‑precision types that overload operator %. */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* value for offset<1 is irrelevant; safe guard */
        return 1;

    seq_t num = 0;           /* numerator of current sum (reduced) */
    seq_t den = 1;           /* denominator of current sum (reduced) */
    seq_t pow2 = 2;          /* equal to 2^k for current k (starting with k=1) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* denominator of the new term: k * 2^k */
        seq_t term_denom = ((seq_t)k) * pow2;

        /* common divisor of current denominator and term denominator */
        seq_t g1 = seq_gcd(den, term_denom);
        seq_t mul1 = term_denom / g1;   /* term_denom/g1 */
        seq_t mul2 = den      / g1;      /* den/g1 */

        /* reduced denominator (lcm of den and term_denom) */
        seq_t new_den = den / g1 * term_denom;

        /* numerator after adding the new term */
        seq_t new_num = num * mul1 + mul2;

        /* final reduction */
        seq_t g2 = seq_gcd(new_num, new_den);
        num = new_num / g2;
        den = new_den / g2;

        /* prepare 2^(k+1) for the next iteration */
        pow2 = pow2 << 1;    /* equivalent to pow2 *= 2, may overflow naturally */
    }

    return den;   /* reduced denominator of the sum */
}
