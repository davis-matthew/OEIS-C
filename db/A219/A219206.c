/*
 * A219206  Triangle, read by rows, where T(n,k) = binomial(n,k)^k for n>=0,
 *          k=0..n.
 * https://oeis.org/A219206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(row(n), k(n))^k(n) where row(n) =
 *            floor((sqrt(8*n+1)-1)/2) and k(n) = n - row(n)*(row(n)+1)/2
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 49, u64 n <= 84, u128 n <= 145,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */

static inline idx_t isqrt128(__int128 x) {
    /* integer square‑root of a 128‑bit non‑negative value,
       returns floor(sqrt(x)).  */
    idx_t low = 0, high = (idx_t)1ULL << 34; /* > sqrt(max 8*n+1) for n<2^64 */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;  /* floor(sqrt(x)) */
}

seq_t a(idx_t n) {
    /* 1. Find the row that contains the n‑th entry. */
    __int128 tcalc = (__int128)8 * n + 1;
    idx_t t = isqrt128(tcalc);
    idx_t row = (t - 1) >> 1;   /* floor((sqrt(8*n+1)-1)/2) */

    /* 2. Find the column k within that row. */
    idx_t offset = (idx_t)(((__int128)row * (row + 1)) / 2);
    idx_t k = n - offset;

    /* 3. Compute binomial(row, k)^k. */
    seq_t base = binomial(row, k);
    seq_t exp  = (seq_t)k;
    return big_pow(base, exp);
}
