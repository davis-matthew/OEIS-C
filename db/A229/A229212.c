/*
 * A229212  Square array of numerators of t(n,k) = (1+1/(k*n))^n, read by
 *          descending antidiagonals.
 * https://oeis.org/A229212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, tabl, nonn
 * Formula:   a(n) = (i*j + 1)^i where (i,j) are the row and column indices of
 *            the nth element read by descending antidiagonals of the square
 *            array t(k,n) = (1 + 1/(k*n))^n. The indices i and j satisfy i >=
 *            1, j >= 1 and i + j = s for some s such that the cumulative
 *            number of terms up to diagonal s-1 is less t...
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */

/* external types supplied by the build system */
extern seq_t big_pow(seq_t base, seq_t exp);

seq_t a(idx_t n)
{
    /* Find the smallest diagonal index s (>=2) such that
       (s-1)*s/2 >= n.  We use a simple incremental scan; for n up to
       10 000 this is trivially fast.  For larger n the loop still runs
       in O(s) ~ O(sqrt(n)). */
    idx_t s = 2;
    __uint128_t tri = 1;    /* (s-1)*s/2 for s==2 */
    while (tri < n) {
        ++s;
        tri = (__uint128_t)(s - 1) * s / 2;
    }

    /* Number of terms that appear before diagonal s */
    __uint128_t prev_tri = (__uint128_t)(s - 2) * (s - 1) / 2;
    idx_t prev = (idx_t)prev_tri;

    /* Position of the term within diagonal s (1‑based).  The order
       inside a diagonal is i = 1,2,…,s-1 (rows increase). */
    idx_t i = n - prev;
    idx_t j = s - i;

    /* Compute base = i*j + 1 without overflow: use 128‑bit intermed. */
    __uint128_t prod = (__uint128_t)i * j;
    seq_t base = (seq_t)(prod + 1);

    /* Result = base^i  (numerator of (1+1/(j*i))^i). */
    return big_pow(base, (seq_t)i);
}
