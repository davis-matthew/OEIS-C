/*
 * A195506  Denominator of Sum_{k=1..n} H(k)/k^2, where H(k) is the k-th
 *          harmonic number.
 * https://oeis.org/A195506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of Σ_{k=1..n} H(k)/k^2 where H(k)=1+1/2+…+1/k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* Handle only n >= 1; undefined behavior for n == 0 */
    if (n == 0) return (seq_t)1;

    /* Current harmonic number H(k) = Hnum/Hden (starts with H(1)=1/1) */
    seq_t Hnum = (seq_t)1;
    seq_t Hden = (seq_t)1;

    /* Accumulated sum S = Snum/Sden (starts with 0/1) */
    seq_t Snum = (seq_t)0;
    seq_t Sden = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t k_seq   = (seq_t)k;
        seq_t term_den   = Hden * k_seq * k_seq;   /* Hden * k^2  */
        seq_t term_num   = Hnum;                   /* Hnum        */

        /* Add term to S: S = S + term_num/term_den  */
        seq_t g1   = seq_gcd(Sden, term_den);
        seq_t lcm  = Sden / g1 * term_den;
        seq_t a1   = Snum * (term_den / g1);
        seq_t a2   = term_num * (Sden / g1);
        seq_t num  = a1 + a2;

        seq_t g2   = seq_gcd(num, lcm);
        Snum = num / g2;
        Sden = lcm / g2;

        /* Update harmonic number to H(k+1) = H(k) + 1/(k+1) */
        idx_t nextk = k + 1;
        seq_t nk    = (seq_t)nextk;
        seq_t hnum  = Hnum * nk + Hden;
        seq_t hden  = Hden * nk;
        seq_t g3    = seq_gcd(hnum, hden);
        Hnum = hnum / g3;
        Hden = hden / g3;
    }

    return Sden;
}
