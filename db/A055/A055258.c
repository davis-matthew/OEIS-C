/*
 * A055258  Sums of two powers of 7.
 * https://oeis.org/A055258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 7^trinv(n) + 7^(n - trinv(n)) where trinv(n) =
 *            floor((sqrt(8*n + 1) - 1) / 2) (largest k with k(k+1)/2 ≤ n)
 *            i.e. a(n) = 7^k + 7^m where k and m satisfy k(k+1)/2 + m = n and
 *            0 ≤ m ≤ k
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 77, u64 n <= 275,
 *            u128 n <= 1080, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Internal helper: binary search for the largest k with k(k+1)/2 ≤ n */
static idx_t triangular_root(idx_t n)
{
    idx_t lo = 0, hi = n;           /* hi safely ≥ the answer */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;          /* upper mid */
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Internal helper: fast exponentiation of base^exp for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp) {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the largest k such that k(k+1)/2 ≤ n (trinv(n)) */
    idx_t k = triangular_root(n);
    unsigned __int128 t = (unsigned __int128)k * (k + 1) / 2;
    idx_t m = n - (idx_t)t;                /* second exponent */

    seq_t term1 = pow_seq_t((seq_t)7, k);
    seq_t term2 = pow_seq_t((seq_t)7, m);
    return term1 + term2;
}
