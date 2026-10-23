/*
 * A158466  Numerators of EH(n), the expected value of the height of a
 *          probabilistic skip list with n elements and p=1/2.
 * https://oeis.org/A158466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = -Sum_{k=1..n} (-1)^k * binom(n,k) / (1-(1/2)^k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*             = -Sum_{k=1..n} (-1)^k * binom(n,k) * 2^k / (2^k-1)    */

#include <stdint.h>
#include <stddef.h>

/* Helper: gcd for seq_t   (works for built‑in and big‑int types) */
static inline seq_t
gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Helper: compare two seq_t values */
static inline int
compare_seq(seq_t a, seq_t b)
{
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/*---------------------------------------------------------------
 *  seq_t a(idx_t n)
 *  Computes the nth numerator of EH(n) for a probabilistic skip
 *  list with p = 1/2.
 *--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == (idx_t)0)                 /* a(0) = 0 */
        return (seq_t)0;

    /* Sum = Σ_{k=1..n} (-1)^k * C(n,k) * 2^k / (2^k-1)   (no outer -) */
    seq_t sum_num   = (seq_t)0;        /* abs value of the running sum   */
    seq_t sum_den   = (seq_t)1;        /* denominator (always positive)   */
    int   sum_sign  = +1;              /* +1 for positive, -1 for negative */

    seq_t binom  = (seq_t)1;   /* C(n,0)   */
    seq_t pow2   = (seq_t)1;   /* 2^0      */

    for (idx_t k = (idx_t)1; k <= n; ++k) {
        /* Update 2^k and binomial coefficient C(n,k) */
        pow2 <<= 1;                            /* pow2 = 2^k      */
        binom *= (seq_t)(n - k + 1);           /* binom *= (n-k+1)*/
        binom /= (seq_t)k;                     /* binom /= k      */

        /* term = (-1)^k * binom * 2^k / (2^k-1)  (term_sign = ±1) */
        seq_t term_num_abs = binom * pow2;     /* |term numerator|    */
        seq_t term_den     = pow2 - ((seq_t)1);/* denominator   */
        int   term_sign    = ((k & 1UL) ? -1 : +1);  /* (-1)^k */

        /* Reduce term fraction before adding */
        seq_t g_term = gcd_seq(term_num_abs, term_den);
        term_num_abs /= g_term;
        term_den      /= g_term;

        /* Add term to the running sum:
           sum = sum_sign * sum_num / sum_den
           +  term_sign * term_num_abs / term_den               */
        seq_t g = gcd_seq(sum_den, term_den);
        seq_t sum_den_over_g = sum_den / g;
        seq_t term_den_over_g = term_den / g;

        seq_t temp_num1 = sum_num * term_den_over_g;   /* sum numerator with common denom */
        seq_t temp_num2 = term_num_abs * sum_den_over_g;/* term numerator with common denom */

        seq_t new_num_abs;
        int   new_sign;

        if (sum_sign == term_sign) { /* same sign: just add */
            new_num_abs = temp_num1 + temp_num2;
            new_sign    = sum_sign;
        } else {                    /* opposite signs: subtract */
            if (compare_seq(temp_num1, temp_num2) >= 0) { /* |temp_num1| >= |temp_num2| */
                new_num_abs = temp_num1 - temp_num2;
                new_sign    = sum_sign;
            } else {
                new_num_abs = temp_num2 - temp_num1;
                new_sign    = term_sign;
            }
        }

        seq_t new_den = sum_den_over_g * term_den;   /* lcm of denominators */

        /* Reduce the new fraction */
        seq_t g_final = gcd_seq(new_num_abs, new_den);
        new_num_abs /= g_final;
        new_den      /= g_final;

        /* Ensure a zero sum has positive sign */
        if (new_num_abs == (seq_t)0)
            new_sign = +1;

        /* Update running sum */
        sum_num  = new_num_abs;
        sum_den  = new_den;
        sum_sign = new_sign;
    }

    /* For n ≥ 1, the computed sum (without outer '-') is negative:
       EH(n) = -sum, so the numerator of EH(n) is sum_num. */
    return sum_num;
}
