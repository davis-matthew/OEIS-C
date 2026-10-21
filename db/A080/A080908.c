/*
 * A080908  a(n) = the sign of r(n), where r(n) is the integer in [ -2n,2n]
 *          which is congruent to (2n)! modulo 4n+1.
 * https://oeis.org/A080908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sign of r(n), where r(n) is the integer in [-2n,2n]
 *            congruent to (2n)! modulo p = 4n+1; for composite p, a(n)=0
 *            except n=2
 * Range:     i8 n <= 33, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t p = (seq_t)4 * (seq_t)n + 1;      /* modulus p = 4n+1 */
    if (n == 0) return 0;                  /* a(0)=0 */

    /* special treatment for n=2, where p=9 and (2n)! is not divisible by 9 */
    if (n == 2) {
        seq_t fact = 1;
        for (idx_t i = 2; i <= 2 * n; ++i)
            fact = (fact * (seq_t)i) % p;
        seq_t r = (fact <= (seq_t)(2 * n)) ? fact : fact - p;
        return (r == 0) ? 0 : (r > 0 ? 1 : -1);
    }

    /* Test if p is prime */
    bool is_prime = (p > 1);
    for (idx_t d = 2; (idx_t)d * d <= p; ++d)
        if (p % d == 0) { is_prime = false; break; }

    /* For composite p (except n==2 already handled) the sequence is 0 */
    if (!is_prime) return 0;

    /* Compute (2n)! modulo p */
    seq_t fact = 1;
    for (idx_t i = 2; i <= 2 * n; ++i)
        fact = (fact * (seq_t)i) % p;

    /* Adjust into the interval [-2n, 2n] */
    seq_t r = (fact <= (seq_t)(2 * n)) ? fact : fact - p;

    /* Return the sign of r */
    if (r == 0) return 0;
    return (r > 0) ? 1 : -1;
}
