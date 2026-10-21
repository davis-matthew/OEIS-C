/*
 * A091894  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n, having k ddu's [here u = (1,1) and d = (1,-1)].
 * https://oeis.org/A091894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where (r,k) are the row and column of the
 *            flattened triangle of OEIS A091894. T(r,k) = 2^(r-2k-1) *
 *            binomial(r-1,2k) * binomial(2k,k)/(k+1). See OEIS for details
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 114, u64 n <= 368,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>            /* for size_t */
#include "../utils/binomial.h" /* seq_t binomial(idx_t n, idx_t k) */

#ifdef __cplusplus
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp) */
#endif

static inline idx_t row_len(idx_t r)
{
    return (r == 0) ? 1 : ((r + 1) / 2);
}

seq_t a(idx_t n)
{
    /* Find the row r and column k that correspond to the nth entry. */
    idx_t r = 0;
    idx_t cum = 0;                       /* elements before current row */
    while (1) {
        idx_t len = row_len(r);
        if (cum + len > n)
            break;
        cum += len;
        r++;
    }
    idx_t k = n - cum;                    /* column within row r */

    /* Special case T(0,0) = 1. */
    if (r == 0)
        return (seq_t)1;

    /* Compute components of the formula. */
    idx_t exp = r - 2 * k - 1;            /* exponent for 2^exp */

    /* binomial(r-1, 2k) */
    seq_t bin = binomial(r - 1, 2 * k);

    /* Catalan(k) = binomial(2k, k)/(k+1) */
    seq_t cat_num = binomial(2 * k, k);
    seq_t cat = cat_num / ((seq_t)(k + 1));

    /* 2^exp.  Use built‑in pow2 if available (C++ case). */
    seq_t powv;
#ifdef __cplusplus
    powv = pow2(exp);
#else
    /* Use a 128‑bit intermediate to avoid shifting beyond the type limits. */
    unsigned __int128 pow128 = 1;
    if (exp < 128)
        pow128 <<= exp;                      /* safe shift for exp < 128 */
    else {
        /* For very large exp the result cannot fit in 64 bits; stop once
           we overflow 128 bits.  This never affects the checks in the
           64‑bit build because such terms do not fit in 64 bits. */
        for (idx_t i = 0; i < exp; ++i) {
            if (pow128 > ((unsigned __int128)~((unsigned __int128)0) >> 1))
                break;
            pow128 <<= 1;
        }
    }
    powv = (seq_t)pow128;
#endif

    /* Multiply the three factors safely.  For the C++ big‑integer build
       this uses the natural big‑integer multiplication.  For the C
       integer build we use 128‑bit arithmetic to avoid overflow. */
#ifdef __cplusplus
    seq_t prod = bin * cat;
    prod *= powv;
    return prod;
#else
    unsigned __int128 prod = (unsigned __int128)bin * cat;
    prod = prod * powv;
    return (seq_t)prod;
#endif
}
