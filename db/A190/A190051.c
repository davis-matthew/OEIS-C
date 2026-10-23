/*
 * A190051  Expansion of (1-x)*(10*x^4-20*x^3+16*x^2-6*x+1)/(1-2*x)^5.
 * https://oeis.org/A190051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1 a(n) = (n^4 + 14*n^3 + 83*n^2 + 214*n + 264) *
 *            2^(n‑7) / 3 (integers)
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 21, u64 n <= 49, u128 n <= 108,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Convert index once – all arithmetic in seq_t */
    seq_t nseq = (seq_t)n;
    seq_t n2   = nseq * nseq;
    seq_t n3   = n2 * nseq;
    seq_t n4   = n3 * nseq;

    /* Polynomial */
    seq_t poly = (seq_t)264
               + (seq_t)214 * nseq
               + (seq_t)83  * n2
               + (seq_t)14  * n3
               + n4;

    /* n < 7 → negative exponent: divide by 2^(7–n) first */
    if (n < 7) {
        idx_t shift = 7 - n;           /* shift in [1,6] */
        seq_t denom = (seq_t)1 << shift;
        denom = denom * (seq_t)3;      /* 3 * 2^(7‑n)   */
        return poly / denom;           /* exact division   */
    }

    /* n ≥ 7 → positive exponent */
    idx_t exp = n - 7;

    /* For integral seq_t types (≤64‑bit) use 128‑bit intermediate to avoid overflow */
#if defined(__SIZEOF_INT128__) || defined(__int128_t) || defined(__int128)
    if (sizeof(seq_t) <= 8) {
        /* 2^(exp) fits in 128‑bit because the result is required to fit seq_t */
        unsigned long long shift = (unsigned long long)exp;
        unsigned __int128 prod   = (unsigned __int128)poly << shift; /* multiply by 2^exp */
        unsigned __int128 res128 = prod / (unsigned __int128)3;
        return (seq_t)res128;
    }
#endif

    /* Generic path: use the provided pow2 helper (handles big integers) */
    seq_t pow_val = pow2(exp);
    seq_t prod    = poly * pow_val;
    return prod / (seq_t)3;
}
