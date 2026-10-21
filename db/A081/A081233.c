/*
 * A081233  Let p = n-th prime, take smallest solution (x,y) to the Pellian
 *          equation x^2 - p*y^2 = 1 with x and y >= 1; sequence gives value
 *          of x.
 * https://oeis.org/A081233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 28, u64 n <= 58, u128 n <= 171,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = the least x ≥ 1 such that
 * x² – p·y² = 1 with p = n‑th prime,  y ≥ 1.
 * The function returns this x for any n ≥ 1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"     /* get_prime(idx_t k) returns the k‑th prime */
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t) */

static seq_t fundamental_solution(idx_t p)
{
    seq_t sp = (seq_t)p;                    /* the prime as seq_t  */
    seq_t a0 = floor_sqrt(sp);              /* integer part of √p   */

    seq_t m   = 0;
    seq_t d   = 1;
    seq_t a   = a0;

    seq_t hprev = 1;
    seq_t h     = a;
    seq_t kprev = 0;
    seq_t k     = 1;

    /* Check trivial solution (never happens for primes, but safe) */
    seq_t left  = h * h;
    seq_t right = sp * k * k + 1;
    if (left == right) return h;

    /* Continue with the continued‐fraction convergents */
    while (1) {
        /* next terms of the period:  m → a → d   */
        m = d * a - m;
        seq_t m2 = m * m;
        d = (sp - m2) / d;
        a = (a0 + m) / d;

        /* update the convergents (h/k) */
        seq_t hnew = a * h + hprev;
        seq_t knew = a * k + kprev;
        hprev = h; kprev = k;
        h = hnew;  k = knew;

        /* check for the fundamental solution */
        left  = h * h;
        right = sp * k * k + 1;
        if (left == right) return h;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but never called */
    idx_t prime = get_prime(n);      /* n‑th prime (uint32_t) */
    return fundamental_solution(prime);
}
