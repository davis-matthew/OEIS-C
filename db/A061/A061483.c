/*
 * A061483  Numerator of 1 + 1/2 + 2/3 + 3/4 + ... + (n-1)/n.
 * https://oeis.org/A061483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the reduced fraction * 1 + 1/2 + 2/3 + … +
 *            (n-2)/(n-1) (for n ≥ 1) * The result is the numerator after
 *            simplification. * The computation uses an O(n) recurrence with
 *            gcd reduction
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Euclidean algorithm – works for any integer type that supports % and /. */
static seq_t gcd_seq_t(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Handle the first two terms explicitly. */
    if (n == 1 || n == 2) {
        return 1;
    }

    /* Current reduced fraction value of 1 + Σ_{k=2}^{i-1} (k-1)/k  (i starts from 2). */
    seq_t num = 1;  /* numerator */
    seq_t den = 1;  /* denominator */

    idx_t i;
    for (i = 2; i <= n - 1; ++i) {
        seq_t term_num = (seq_t)(i - 1);  /* (k-1) */
        seq_t term_den = (seq_t)i;        /* k       */

        /* Add the new term: (num/den) + (term_num/term_den) */
        seq_t new_num = num * term_den + den * term_num;
        seq_t new_den = den * term_den;

        /* Reduce the fraction */
        seq_t g = gcd_seq_t(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }

    /* Return the reduced numerator. */
    return num;
}
