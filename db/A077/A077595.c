/*
 * A077595  Numerator of integral from 0 to 1 of (1 + x^2)^n, in lowest terms.
 * https://oeis.org/A077595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of Σ_{k=0..n} binomial(n,k)/(2k+1) in lowest
 *            terms
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simple Euclidean gcd for seq_t (works for builtin integer types and for
   arbitrary-precision integer classes that overload the required operators). */
static seq_t gcd_seq_t(seq_t a, seq_t b)
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
    seq_t sum_num = 0;          /* numerator of the running sum   */
    seq_t sum_den = 1;          /* denominator of the running sum */
    seq_t binom   = 1;          /* C(n,0)                          */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term_num = binom;
        seq_t term_den = (seq_t)(2 * k + 1);          /* denominator of the term */

        /* Reduce the term and find lcm factor for addition */
        seq_t g  = gcd_seq_t(sum_den, term_den);
        seq_t l  = sum_den / g;
        seq_t t  = term_den / g;

        /* Add fractions: (sum_num/l) + (term_num/t) */
        seq_t part1 = sum_num * t;
        seq_t part2 = term_num * l;
        seq_t new_num = part1 + part2;
        seq_t new_den = l * term_den;                 /* l * term_den = (sum_den/g) * term_den */

        /* Reduce the resulting fraction to lowest terms */
        seq_t g2 = gcd_seq_t(new_num, new_den);
        sum_num = new_num / g2;
        sum_den = new_den / g2;

        /* Prepare next binomial coefficient C(n,k+1) */
        if (k < n)
        {
            seq_t mult = (seq_t)(n - k);
            seq_t div  = (seq_t)(k + 1);
            binom = (binom * mult) / div;           /* exact division */
        }
    }

    return sum_num;   /* numerator in lowest terms */
}
