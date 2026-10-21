/*
 * A074949  Denominators of Sum_{k=1..n} 1/lcm(n,k).
 * https://oeis.org/A074949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator of S(n) where S(n) = Σ_{k=1..n} 1 / lcm(n,k)
 *            (in lowest terms)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute L = lcm(1,2,…,n).  Then  
   S(n) = ( Σ_{k=1..n} L / lcm(n,k) ) / L.  
   The fraction is reduced by dividing numerator and denominator by their gcd. */

#include <stdint.h>
#include <stdbool.h>

/* GCD for type seq_t – works for all supported seq_t */
static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute denominator of the sum.  Argument n is 1‑based. */
seq_t a(idx_t n)
{
    /* Compute L = lcm(1,2,…,n). */
    seq_t L = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t g = gcd_seq_t(L, ii);
        L = (L / g) * ii;          /* exact, no overflow on supported seq_t */
    }

    /* Sum the numerators: Σ_{k=1..n} L / lcm(n,k). */
    seq_t sum_num = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t g = gcd_seq_t((seq_t)n, kk);
        seq_t lcm_nk = ((seq_t)n * kk) / g;   /* lcm(n,k) */
        seq_t add = L / lcm_nk;              /* integer division: lcm divides L */
        sum_num += add;
    }

    /* Reduce the fraction sum_num / L.  The denominator of the sum is L / gcd. */
    seq_t g = gcd_seq_t(sum_num, L);
    return L / g;
}
