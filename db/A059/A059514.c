/*
 * A059514  For a rational number p/q let f(p/q) = p*q divided by (the sum of
 *          digits of p and of q) minus 1; a(n) is obtained by iterating f,
 *          starting at n/1, until an integer is reached, or if no integer is
 *          ever reached then a(n) = 0.
 * https://oeis.org/A059514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 14, u32 n <= 30, u64 n <= 1318,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is obtained by iterating f(p/q) = (p*q) / (S(p)+S(q)-1) 
 * where S(x) is the sum of decimal digits of x.  Start with n/1, keep the
 * fraction in lowest terms after each step, and return the first integer
 * reached (i.e., q == 1).  If the iteration enters a cycle that never
 * hits an integer, return 0.
 */
#include "../utils/gcd.h"
#include "../utils/digit_sum.h"

/* Compute the next fraction after applying f.  The result is reduced. */
static inline void
next_frac(seq_t p, seq_t q, seq_t *p2, seq_t *q2)
{
    seq_t prod = p * q;
    seq_t s    = digit_sum(p) + digit_sum(q);
    seq_t den  = s - (seq_t)1;
    seq_t g    = gcd(prod, den);
    *p2 = prod / g;
    *q2 = den  / g;
}

/* OEIS entry function. */
seq_t
a(idx_t n)
{
    seq_t p = (seq_t)n;
    seq_t q = (seq_t)1;

    /* Perform the first two steps of the iteration. */
    seq_t tort_p, tort_q, hare_p, hare_q;

    next_frac(p, q, &tort_p, &tort_q);
    if (tort_q == (seq_t)1)
        return tort_p;

    next_frac(tort_p, tort_q, &hare_p, &hare_q);
    if (hare_q == (seq_t)1)
        return hare_p;

    /* Floyd's cycle detection. */
    while (1) {
        /* One step for the tortoise. */
        next_frac(tort_p, tort_q, &tort_p, &tort_q);
        if (tort_q == (seq_t)1)
            return tort_p;

        /* Two steps for the hare. */
        next_frac(hare_p, hare_q, &hare_p, &hare_q);
        if (hare_q == (seq_t)1)
            return hare_p;
        next_frac(hare_p, hare_q, &hare_p, &hare_q);
        if (hare_q == (seq_t)1)
            return hare_p;

        /* Cycle detected without reaching an integer. */
        if (tort_p == hare_p && tort_q == hare_q)
            return (seq_t)0;
    }
}
