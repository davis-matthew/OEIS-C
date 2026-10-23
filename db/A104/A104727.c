/*
 * A104727  Triangle T(n,k) = (k-1-n)*(k-2-n)*(k^2+k+2*k*n+3*n^2+5*n)/24 read
 *          by rows, 1<=k<=n.
 * https://oeis.org/A104727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row, col) where row,col are derived from n: T(r,k) = (r
 *            - k + 1)*(r - k + 2)*(k^2 + k + 2*k*r + 3*r^2 + 5*r)/24
 *            (converted from the OEIS definition)
 * Range:     u8 n <= 15, u16 n <= 351, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned long long gcd64(unsigned long long a, unsigned long long b)
{
    while (b) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline void reduce_factor(seq_t *f, unsigned long long *den)
{
    unsigned long long fv = (unsigned long long)*f;
    unsigned long long g = gcd64(fv, *den);
    fv /= g;
    *f = (seq_t)fv;
    *den /= g;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Column index (1‑based) within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    seq_t rn = (seq_t)r;
    seq_t ck = (seq_t)k;

    /* Numerator factors */
    seq_t a_f = rn + 1 - ck;                                  /* n - k + 1 */
    seq_t b_f = rn + 2 - ck;                                  /* n - k + 2 */
    seq_t c_f = ck * ck + ck + 2 * ck * rn + 3 * rn * rn + 5 * rn;

    unsigned long long denom = 24;

    reduce_factor(&a_f, &denom);
    if (denom > 1) reduce_factor(&b_f, &denom);
    if (denom > 1) reduce_factor(&c_f, &denom);   /* denom must now be 1 */

    seq_t result = a_f * b_f;
    result = result * c_f;
    return result;
}
