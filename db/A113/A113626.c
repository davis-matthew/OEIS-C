/*
 * A113626  Numbers simultaneously heptagon-free, pentagon-free, squarefree
 *          and triangle-free.
 * https://oeis.org/A113626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n‑th positive integer that * has no divisor of the
 *            form b(b+1)/2 (b≥2), * d(3d−1)/2 (d≥2), * e(5e−3)/2 (e≥2), and *
 *            is square‑free (no square prime factor)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether v is square‑free.  For every integer p≥2,
 * if p² divides v we return false; otherwise true.  The
 * loop terminates when p² > v. */
static bool is_squarefree(seq_t v)
{
    seq_t p = (seq_t)2;
    for (;; ++p) {
        seq_t p2 = p * p;
        if (p2 > v) break;
        if (v % p2 == 0) return false;
    }
    return true;
}

/* Test whether val satisfies the restrictions of A113626. */
static bool is_valid(seq_t val)
{
    if (!is_squarefree(val)) return false;

    /* Triangular numbers: b*(b+1)/2 for b≥2. */
    for (idx_t b = 2; ; ++b) {
        seq_t t = (seq_t)b * ((seq_t)b + 1) / 2;
        if (t > val) break;
        if (val % t == 0) return false;
    }

    /* Pentagonal numbers: d*(3*d−1)/2 for d≥2. */
    for (idx_t d = 2; ; ++d) {
        seq_t p = (seq_t)d * (3 * (seq_t)d - 1) / 2;
        if (p > val) break;
        if (val % p == 0) return false;
    }

    /* Heptagonal numbers: e*(5*e−3)/2 for e≥2. */
    for (idx_t e = 2; ; ++e) {
        seq_t h = (seq_t)e * (5 * (seq_t)e - 3) / 2;
        if (h > val) break;
        if (val % h == 0) return false;
    }

    return true;
}

/* Return the n-th term of sequence A113626 (n≥1). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t num   = 0;

    while (count < n) {
        ++num;
        if (is_valid(num)) ++count;
    }
    return num;
}
