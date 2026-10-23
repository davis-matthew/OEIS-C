/*
 * A199514  Numerators of zeros to a symmetric polynomial. Numerators of
 *          mu(n)^2*(n/(n - phi(n))).
 * https://oeis.org/A199514
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = μ(n)^2 * n / gcd(n, n - φ(n)) (n ≥ 2)
 * Range:     u8 n <= 256, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* gcd for arbitrary seq_t */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* offset 2 – only defined for n ≥ 2 */

    idx_t tmp = n;
    bool squarefree = true;
    seq_t phi = 1;

    /* factor 2 */
    if ((tmp & 1) == 0) {
        idx_t exp = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            exp++;
        }
        if (exp > 1) squarefree = false;
        phi *= ((seq_t)2 - 1);
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            if (exp > 1) squarefree = false;
            phi *= ((seq_t)p - 1);
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        phi *= ((seq_t)tmp - 1);
    }

    if (!squarefree) return (seq_t)0;

    seq_t n_seq = (seq_t)n;
    seq_t d = n_seq - phi;                 /* n - φ(n) > 0 for n ≥ 2 */
    seq_t g = gcd_seq(n_seq, d);
    return n_seq / g;
}
