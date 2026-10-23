/*
 * A173882  Triangle T(n, k) = A090443(n-1)/(A090443(k-1)*A090443(n-k-1)) read
 *          by rows.
 * https://oeis.org/A173882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = ⌊(√(8n+1)−1)/2⌋ and k = n − r(r+1)/2 ;
 *            T(r,k) = 2 * binom(r−1,k−1) * binom(r,k) * binom(r+1,k+1) *
 *            (r−k)/(r−k+1), with T(r,0) = T(r,r) = 1
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 96, u64 n <= 310,
 *            u128 n <= 1101, big any n
 * Helpers:   binom, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/floor_sqrt.h"

static inline seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* locate the triangle row r and position k */
    seq_t t = (seq_t)8 * (seq_t)n + 1;
    seq_t s = floor_sqrt(t);
    idx_t r = (idx_t)((s - 1) / 2);
    idx_t start = r * ((idx_t)r + 1) / 2;
    idx_t k = n - start;

    /* boundary entries of each row are 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    seq_t D = (seq_t)(r - k + 1);    /* denominator of the formula */
    seq_t result = 1;

    /* factor 1: binom(r-1,k-1) */
    seq_t f = binom(r - 1, k - 1);
    seq_t g = gcd_seq_t(f, D);
    f /= g;
    D /= g;
    result *= f;

    /* factor 2: binom(r,k) */
    f = binom(r, k);
    g = gcd_seq_t(f, D);
    f /= g;
    D /= g;
    result *= f;

    /* factor 3: binom(r+1,k+1) */
    f = binom(r + 1, k + 1);
    g = gcd_seq_t(f, D);
    f /= g;
    D /= g;
    result *= f;

    /* factor 4: the multiplicative constant 2 */
    f = 2;
    g = gcd_seq_t(f, D);
    f /= g;
    D /= g;
    result *= f;

    /* factor 5: (r-k) */
    f = (seq_t)(r - k);
    g = gcd_seq_t(f, D);
    f /= g;
    D /= g;
    result *= f;

    /* any remaining denominator (should be 1) */
    if (D > 1)
        result /= D;

    return result;
}
