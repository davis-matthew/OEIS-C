/*
 * A154630  A triangular sequence of polynomial coefficients: {a,b,c,d}={4, 5,
 *          5, 0}; p(x,n)=(-1)^(n)*(1 - d - c x)^(n + 1)*Sum[(a*k + b)^n*(c*x
 *          + d)^k, {k, 0, Infinity}].
 * https://oeis.org/A154630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^(m) in
 *            p(x,n)=(-1)^n*(1-5x)^(n+1)*∑_{k≥0}(4k+5)^n*(5x)^k, where m is
 *            determined by row‑major enumeration: r = ⌊(√(8n+1)-1)/2⌋, c = n
 *            - r(r+1)/2. Then a(n)=t(r,c)=5^c * Σ_{j=0}^{min(c,r+1)}
 *            (-1)^(r+j) * C(r+1,j) * (4(c-j)+5)^r. All operations are carried
 *            out in the integer...
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 30, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for uint64_t */

/* integer square root: largest s with s*s <= n, 0 ≤ n ≤ 2^64-1 */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

/* integer exponentiation by repeated squaring: base^exp */
static inline seq_t pow_seq_big(seq_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t b   = base;
    idx_t e   = exp;
    while (e > 0) {
        if (e & 1)
            res = res * b;
        b = b * b;
        e >>= 1;
    }
    return res;
}

/* Computes the n-th term of OEIS A154630 */
seq_t a(idx_t n)
{
    /* determine row r and column c of (n) in the triangular array */
    idx_t tmp = (idx_t)(((__int128)8 * n) + 1);   /* 8*n+1, no overflow for n≈10^4 */
    idx_t s   = floor_sqrt_u64(tmp);            /* floor(sqrt(8n+1)) */
    idx_t r   = (s - 1) / 2;                     /* row index */
    idx_t c   = n - r * (r + 1) / 2;            /* column index in that row */

    idx_t limit = c;
    if (limit > r + 1) limit = r + 1;          /* j runs 0 .. min(c,r+1) */

    seq_t bin = 1;            /* C(r+1,0) */
    seq_t sum = 0;

    for (idx_t j = 0; j <= limit; ++j) {
        /* compute (4(c-j)+5)^r */
        idx_t base_val = 4 * (c - j) + 5;
        seq_t base_pow = pow_seq_big((seq_t)base_val, r);

        /* term = C(r+1,j) * base_pow with sign (-1)^j */
        seq_t term = bin * base_pow;
        if (j & 1) /* odd j gives minus sign */
            term = -term;

        sum += term;

        if (j < limit) {
            /* update bin to C(r+1,j+1) */
            bin = bin * (r + 1 - j);
            bin = bin / (j + 1);
        }
    }

    /* overall sign (-1)^r */
    if (r & 1)
        sum = -sum;

    /* multiply by 5^c */
    seq_t pow5col = pow_seq_big((seq_t)5, c);
    return sum * pow5col;
}
