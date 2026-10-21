/*
 * A054618  Triangle T(n,k) = Sum_{d|n} phi(d)*k^(n/d).
 * https://oeis.org/A054618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row and k is the column in the
 *            triangle * T(r,k) = Σ_{d|r} φ(d) · k^{r/d} ; the index n
 *            enumerates the triangle * row by row (row 1: k=1; row 2: k=1,2;
 *            row 3: k=1,2,3; …)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --------- helper: integer square root (high‑precision) --------- */
static inline idx_t isqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 r = 0, bit = (unsigned __int128)1 << 126; /* biggest power of 2 <= 2^127 */
    while (bit) {
        unsigned __int128 next = r + bit;
        if (next * next <= v) r = next;
        bit >>= 1;
    }
    return (idx_t)r;
}

/* --------- Euler's totient for unsigned integers (small) -------- */
static inline seq_t phi_u64(idx_t n)
{
    seq_t result = (seq_t)n;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return result;
}

/* --------- fast exponentiation of seq_t base to idx_t exponent -------- */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* --------- find row r (smallest with r*(r+1)/2 >= n) --------- */
static inline idx_t find_row(idx_t n)
{
    // binary search on r
    idx_t low = 1, high = 2;
    while ((((unsigned __int128)high * (high + 1)) / 2) < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 half = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (half >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* --------- main sequence function --------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for n==0, offset is 1 */

    /* locate row r and column k */
    idx_t r = find_row(n);
    unsigned __int128 prev_terms = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - (idx_t)prev_terms;   /* column index (1‑based) */

    /* compute T(r,k) = Σ_{d|r} φ(d) * k^{r/d} */
    seq_t total = (seq_t)0;

    for (idx_t d = 1; d * d <= r; ++d) {
        if (r % d == 0) {
            idx_t exp1 = r / d;
            seq_t term1 = phi_u64(d) * pow_seq_t((seq_t)k, exp1);
            total += term1;
            if (d != r / d) {
                idx_t d2 = r / d;
                idx_t exp2 = r / d2;            /* which equals d */
                seq_t term2 = phi_u64(d2) * pow_seq_t((seq_t)k, exp2);
                total += term2;
            }
        }
    }

    return total;
}
