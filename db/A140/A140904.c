/*
 * A140904  Number of 4 X 6 matrices with elements in 0..n with each row and
 *          each column in nondecreasing order. 4,6,n can be permuted, see
 *          formula.
 * https://oeis.org/A140904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=0}^{3} ( binomial(n+9, n+i) * i! / (n+9-i)^(3-i) )
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom, my_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binomial utility and a light‑weight gcd implementation. */
#include "../utils/binom.h"
#include "../utils/my_gcd.h"

seq_t a(idx_t n)
{
    /* s = n + 9 (since 4 + 6 - 1 = 9) */
    idx_t s = n + 9;

    /* Precomputed factorials for i = 0..3 */
    const seq_t fact[4] = { 1ULL, 1ULL, 2ULL, 6ULL };

    seq_t num = 1;   /* numerator of the running product */
    seq_t den = 1;   /* denominator of the running product */

    for (idx_t i = 0; i < 4; ++i) {
        /* binomial part */
        seq_t bin = binom(s, n + i);

        /* Numerator: bin * i! */
        seq_t term_num = bin;
        term_num *= fact[i];

        /* Denominator: (s - i)^(3-i) */
        seq_t term_den = 1;
        idx_t pow_exp = 3 - i;
        seq_t base = s - i;
        for (idx_t e = 0; e < pow_exp; ++e)
            term_den *= base;

        /* Reduce the individual fraction first */
        seq_t g = my_gcd(term_num, term_den);
        term_num /= g;
        term_den /= g;

        /* Cross‑reduce with the current running product */
        seq_t g1 = my_gcd(num, term_den);
        num /= g1;
        term_den /= g1;

        seq_t g2 = my_gcd(term_num, den);
        term_num /= g2;
        den /= g2;

        /* Multiply the reduced terms in */
        num *= term_num;
        den *= term_den;
    }

    /* The final result is num / den; den should be 1 by construction. */
    return num / den;
}
