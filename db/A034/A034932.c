/*
 * A034932  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 16.
 * https://oeis.org/A034932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7380, u16 n <= 7380, u32 n <= 7380, u64 n <= 7380,
 *            u128 n <= 7380, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) equals the nth entry of Pascal's triangle modulo 16, read row-wise. */
#include <stdint.h>
#include <stddef.h>

/* integer square root: floor(sqrt(x)) for 0 <= x <= ULLONG_MAX */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Find row i and column j containing the n-th element. */
    idx_t D = 1 + (idx_t)8 * n;          /* 1 + 8n */
    idx_t s = isqrt_u64(D);             /* floor(sqrt(1+8n)) */
    idx_t i = (s - (idx_t)1) >> 1;      /* i = floor((sqrt(1+8n)-1)/2) */
    idx_t start = i * (i + (idx_t)1) / (idx_t)2;
    idx_t j = n - start;                /* 0 <= j <= i */

    /* Compute exponent of 2 in C(i,j). */
    idx_t v2i  = i   - __builtin_popcountll(i);
    idx_t v2j  = j   - __builtin_popcountll(j);
    idx_t v2ij = (i - j) - __builtin_popcountll(i - j);
    idx_t exp2 = v2i - v2j - v2ij;      /* >= 0 */

    /* Multiply odd parts of numerator and denominator. */
    seq_t num_odd = 1;
    for (idx_t k = (idx_t)1; k <= j; ++k) {
        idx_t num = i - k + (idx_t)1;
        while ((num & (idx_t)1) == (idx_t)0) num >>= (idx_t)1;
        num_odd = (num_odd * (num & (idx_t)0xF)) & (idx_t)0xF;
    }

    seq_t den_odd = 1;
    for (idx_t k = (idx_t)1; k <= j; ++k) {
        idx_t den = k;
        while ((den & (idx_t)1) == (idx_t)0) den >>= (idx_t)1;
        den_odd = (den_odd * (den & (idx_t)0xF)) & (idx_t)0xF;
    }

    /* Modular inverse of odd denominator modulo 16 (odd numbers are invertible). */
    static const seq_t inv[16] = {0, 1, 0, 11, 0, 13, 0, 7,
                                  0, 9, 0, 3, 0, 5, 0, 15};
    seq_t inv_den   = inv[(size_t)(den_odd & (idx_t)0xF)];
    seq_t odd_part  = (num_odd * inv_den) & (idx_t)0xF;

    /* Multiply by 2^exp2 modulo 16. */
    seq_t pow2 = (exp2 >= 4) ? (seq_t)0
                            : ((seq_t)1 << exp2) & (idx_t)0xF;

    seq_t result = (pow2 * odd_part) & (idx_t)0xF;
    return result;
}
