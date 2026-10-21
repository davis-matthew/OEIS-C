/*
 * A063268  Let f be a function on rationals p/q (p,q coprime) defined by
 *          f(p/q) = abs(p-q)/g(p), where g(p) is the next odd number
 *          (starting with p) that we get after iteration of h(n) = n/2 when n
 *          is even, 5n-1 when n is odd. Start with f(n/1) and iterate f until
 *          it reaches again an integer, which is a(n)....
 * https://oeis.org/A063268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) defined by iterating f(p/q) = |p‑q| / g(p) where g(p) is
 *            the first odd integer obtained by repeatedly applying: h(k) =
 *            k/2 if k even, h(k) = 5k‑1 if k odd, starting from p. a(n) is
 *            the integer reached after starting from the rational n/1 and
 *            repeatedly applying f until the denominator become...
 * Range:     u8 n <= 34, u16 n <= 4751, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   g_of, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/g_of.h"

typedef struct { seq_t p; seq_t q; } fraction_t;

/* Compute the next fraction f(p/q).  The result is stored in *np and *nq. */
static void next_fraction(seq_t p, seq_t q, seq_t *np, seq_t *nq) {
    seq_t num = (p > q) ? p - q : q - p;   /* |p‑q|, p,q are non‑negative */
    seq_t den = g_of(p);
    seq_t g2 = gcd(num, den);
    *np = num / g2;
    *nq = den / g2;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* index 0 is outside the offset */

    /* Start with the rational n/1 and apply f once. */
    seq_t p = n;
    seq_t q = 1;

    seq_t tort_p, tort_q, hare_p, hare_q;

    next_fraction(p, q, &tort_p, &tort_q);
    if (tort_q == 1) return tort_p;          /* reached an integer */

    next_fraction(tort_p, tort_q, &hare_p, &hare_q);
    if (hare_q == 1) return hare_p;

    /* Floyd's cycle detection: tortoise moves one step, hare moves two. */
    while (!(tort_p == hare_p && tort_q == hare_q)) {
        /* advance tortoise by one */
        next_fraction(tort_p, tort_q, &tort_p, &tort_q);
        if (tort_q == 1) return tort_p;

        /* advance hare by two */
        next_fraction(hare_p, hare_q, &hare_p, &hare_q);
        if (hare_q == 1) return hare_p;
        next_fraction(hare_p, hare_q, &hare_p, &hare_q);
        if (hare_q == 1) return hare_p;
    }
    /* If a cycle is detected before an integer, return 0. */
    return 0;
}
