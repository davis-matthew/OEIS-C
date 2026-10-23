/*
 * A173486  a(n) = the largest (n+2)-digit number ending in n+1 zeros that is
 *          divisible by n, else 0.
 * https://oeis.org/A173486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest (n+2)-digit number ending in n+1 zeros that is
 *            divisible by n; if no such number exists, a(n) = 0
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A173486.
   Only standard integer arithmetic is used; no external utilities are required. */

#include <stdint.h>

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    /* Euclidean algorithm for unsigned integers. */
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1. */
    if (n == 0) return 0;

    /* Compute t = 10^(n+1) mod n. */
    seq_t t = 1;
    for (idx_t i = 0; i < n + 1; ++i) {
        t = (t * 10) % (seq_t)n;
    }

    /* Compute g = gcd(n, t). */
    seq_t g = gcd_seq_t((seq_t)n, t);

    /* q = n / g.  If q > 9, no digit d in 1..9 satisfies the divisibility. */
    seq_t q = (seq_t)n / g;
    if (q > 9) return 0;

    /* Largest digit d in 1..9 that is a multiple of q. */
    seq_t d = (9 / q) * q;

    /* Compute 10^(n+1) exactly. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < n + 1; ++i) {
        pow10 *= 10;
    }

    /* Result is d * 10^(n+1). */
    return d * pow10;
}
