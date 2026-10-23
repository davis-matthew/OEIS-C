/*
 * A154429  a(n) is the least k such that the greedy algorithm (for Egyptian
 *          fractions) on 4k/(24n+1) terminates in at most three steps.
 * https://oeis.org/A154429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least k such that the greedy Egyptian fraction
 *            representation of 4k/(24n+1) terminates in at most three steps
 * Range:     u8 n <= 10, u16 n <= 2846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


/* Helper that multiplies two values modulo m.  All arguments are known
   to be smaller than 2^32, so a 64‑bit intermediate is safe. */
static seq_t mul_mod_seq(seq_t x, seq_t y, seq_t m)
{
    return (seq_t)(((uint64_t)x * (uint64_t)y) % (uint64_t)m);
}

seq_t a(idx_t n)
{
    seq_t d = (seq_t)24 * n + 1;   /* denominator */

    for (idx_t kk = 1; ; ++kk)
    {
        seq_t k     = (seq_t)kk;
        seq_t fourk = (seq_t)4 * k;

        /* a = ceil(d / (4k)) */
        seq_t a_val = (d + fourk - 1) / fourk;   /* ceil division */

        /* p1 = 4k * a - d  (always > 0 because 4k is even and d is odd) */
        seq_t p1 = fourk * a_val - d;

        /* r = (d * a) mod p1 */
        seq_t r = mul_mod_seq(d % p1, a_val % p1, p1);

        /* If r == 0 the greedy algorithm stops after two steps */
        if (r == 0)
            return k;

        /* p2 = p1 - r  (always > 0) */
        seq_t p2 = p1 - r;
        if (p2 == 0)  /* safety, should not happen */
            return k;

        /* r_q1 = (d * a) mod p2  (needed for the third step test) */
        seq_t r_q1 = mul_mod_seq(d % p2, a_val % p2, p2);

        /* Compute u2 mod p2 using modular arithmetic */
        seq_t mod_pp = (seq_t)((uint64_t)p1 * (uint64_t)p2);          /* < 2^32 */
        seq_t q1_mod_pp = mul_mod_seq(d % mod_pp, a_val % mod_pp, mod_pp);

        /* diff = (q1_mod_pp - r) mod mod_pp */
        seq_t diff = (q1_mod_pp < r) ? q1_mod_pp + mod_pp - r : q1_mod_pp - r;

        /* t = floor((q1 - r) / p1)  (mod p2) */
        seq_t t_mod_p2 = diff / p1;      /* 0 <= t_mod_p2 < p2 */

        seq_t u2_mod_p2 = (t_mod_p2 + 1) % p2;

        /* Check if (r_q1 * u2_mod_p2) mod p2 == 0 */
        seq_t prod = mul_mod_seq(r_q1, u2_mod_p2, p2);
        if (prod == 0)
            return k;
    }
}
