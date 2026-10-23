/*
 * A120301  Absolute value of numerator of the sum of all elements of the n X
 *          n matrix M with M[i,j] = (-1)^(i+j)*i/j for i,j = 1..n.
 * https://oeis.org/A120301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = | numerator( sum_{i=1..n} sum_{j=1..n} (-1)^(i+j) * i/j )
 *            | = | number (( ceil(n/2) * H_alt(n) ) / gcd( ceil(n/2)*num_H ,
 *            den_H) ) | where H_alt(n) = sum_{k=1..n} (-1)^(k+1)/k = num_H /
 *            den_H in lowest terms. See derivation: A(n)=(-1)^n*ceil(n/2),
 *            B(n)=(-1)^(n+1)*H_alt(n), matrix sum = A(n...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */
#include <stddef.h>         /* for size_t, if needed */
#include <stdint.h>         /* for uint64_t etc.  (build supplies seq_t, idx_t) */

seq_t a(idx_t n)
{
    /* Compute alternating harmonic H_alt(n) = sum_{k=1..n} (-1)^(k+1)/k as reduced fraction num_H/den_H. */
    seq_t num_H = 0;        /* numerator (non‑negative) */
    seq_t den_H = 1;        /* denominator (positive) */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t mult = num_H * (seq_t)k;  /* current numerator multiplied by k */
        /* sign of the k‑th term: + for odd k, – for even k. */
        if (k & 1) {           /* odd k: add den_H */
            mult += den_H;
        } else {               /* even k: subtract den_H */
            mult -= den_H;     /* mult stays non‑negative */
        }
        seq_t new_den = den_H * (seq_t)k;
        seq_t g = gcd(mult, new_den);
        num_H = mult / g;
        den_H = new_den / g;
    }

    /* Compute |A(n)| = ceil(n/2). */
    seq_t A_abs = (n & 1) ? ((seq_t)(n + 1) / 2) : ((seq_t)n / 2);

    /* Result numerator before final reduction. */
    seq_t num_total = A_abs * num_H;

    /* Reduce with denominator. */
    seq_t g2 = gcd(num_total, den_H);
    seq_t result = num_total / g2;
    return result;
}
