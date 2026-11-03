/*
 * A125667  Eta numbers (from the Japanese word for "pariah" or "outcast").
 *          These are the positive odd integers which cannot be used to make a
 *          hypotenuse of a primitive Pythagorean triangle (PPT).
 * https://oeis.org/A125667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Eta numbers (OEIS A125667)
 *
 * An odd integer c is an eta number iff it has at least one prime factor
 * congruent to 3 (mod 4).  Equivalently, c is *not* a product of primes
 * all congruent to 1 (mod 4).  The function below searches successive odd
 * numbers until the n‑th eta number is found.
 */

#include <stdbool.h>
#include <stdint.h>

static bool is_eta(seq_t x)
{
    /* 1 is the first eta number */
    if (x == (seq_t)1)
        return true;

    /* Every candidate we test is odd (we increment by 2 below). */
    seq_t tmp = x;

    /* Trial division by odd primes.  The loop terminates when p*p > tmp. */
    seq_t p = 3;
    while (p * p <= tmp) {
        if (tmp % p == 0) {
            /* If we find a prime ≡ 3 (mod 4), x is definitely an eta number. */
            if (p % 4 == 3)
                return true;
            /* Remove all copies of the current prime. */
            while (tmp % p == 0)
                tmp /= p;
        }
        p += 2;   /* skip even numbers */
    }

    /* If a prime factor larger than sqrt(tmp) remains, it is equal to tmp. */
    if (tmp > (seq_t)1) {
        if (tmp % 4 == 3)
            return true;
        /* tmp % 4 == 1 would mean all prime factors are 1 mod 4 */
    }

    /* No prime factor ≡ 3 (mod 4) was found → not an eta number. */
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* offset not used but harmless */

    idx_t count = (idx_t)0;
    seq_t candidate = (seq_t)1;   /* first odd integer */

    while (count < n) {
        if (is_eta(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += (seq_t)2;   /* advance to next odd integer */
    }

    /* This point is never reached for valid `n` ≥ 1. */
    return (seq_t)0;
}
