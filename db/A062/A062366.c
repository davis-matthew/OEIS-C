/*
 * A062366  Let f be a function on rationals p/q (p,q coprime) defined by
 *          f(p/q) = abs(p-q)/nextoddColl(p) (so the denominator is the next
 *          odd number in the Collatz trajectory of p). Start with f(n/1) and
 *          repeat application of f until it reaches again an integer, which
 *          is a(n). If no integer is reached, then...
 * https://oeis.org/A062366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   nextodd_collatz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A062366 – iterate f(p/q) = |p-q| / nextoddColl(p) starting from n/1
   until the denominator becomes 1 or a cycle is detected.  The function
   returns the integer value reached, or 0 if a cycle is found before an
   integer is reached.  All arithmetic is performed in seq_t, with
   conversions from idx_t performed via (seq_t) cast. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nextodd_collatz.h"

/* Euclidean GCD for seq_t (works for any unsigned integer type). */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Perform one step of the map: from p/q to reduced |p-q| / nextoddColl(p). */
static void step(seq_t p, seq_t q, seq_t *out_p, seq_t *out_q)
{
    seq_t diff = (p > q) ? p - q : q - p;
    seq_t den  = nextodd_collatz(p);
    seq_t g    = gcd_seq_t(diff, den);
    *out_p = diff / g;
    *out_q = den  / g;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;    /* offset is 1, but guard */

    seq_t tort_p = n, tort_q = (seq_t)1;
    step(tort_p, tort_q, &tort_p, &tort_q);
    if (tort_q == (seq_t)1) return tort_p;     /* integer reached immediately */

    /* Hare starts two steps ahead of tortoise. */
    seq_t hare_p = tort_p, hare_q = tort_q;
    step(hare_p, hare_q, &hare_p, &hare_q);
    if (hare_q == (seq_t)1) return hare_p;
    step(hare_p, hare_q, &hare_p, &hare_q);
    if (hare_q == (seq_t)1) return hare_p;

    /* Floyd cycle detection loop. */
    while (!(tort_p == hare_p && tort_q == hare_q)) {
        step(tort_p, tort_q, &tort_p, &tort_q);
        if (tort_q == (seq_t)1) return tort_p;
        step(hare_p, hare_q, &hare_p, &hare_q);
        if (hare_q == (seq_t)1) return hare_p;
        step(hare_p, hare_q, &hare_p, &hare_q);
        if (hare_q == (seq_t)1) return hare_p;
    }

    /* Cycle detected without reaching an integer. */
    return (seq_t)0;
}
