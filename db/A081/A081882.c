/*
 * A081882  Number of steps needed to reach an integer starting with n-1/4 and
 *          iterating the map x ->x*ceiling(x).
 * https://oeis.org/A081882
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps needed to reach an integer starting from
 *            n-¼, iterating the map x → x * ceil(x)
 * Range:     u8 n <= 2, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Euclid's algorithm for arbitrary‑precision intergers. */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Computes a(n) for n ≥ 2. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset: a(2) is the first term */
    seq_t num = ((seq_t)n) * 4ULL - 1;  /* numerator of (n-¼) = (4n-1)/4 */
    seq_t den = 4;                   /* denominator of the fraction */
    idx_t steps = 0;

    while (num % den != 0) {        /* until the fraction becomes integer */
        ++steps;
        seq_t ceil = (num + den - 1) / den;  /* ceil(num/den) */
        num = num * ceil;                     /* multiply by ceil */
        seq_t g = gcd_seq(num, den);          /* reduce the fraction */
        if (g > 1) {
            num /= g;
            den /= g;
        }
    }
    return (seq_t)steps;
}
