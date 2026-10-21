/*
 * A201639  Triangle read by rows, T(n,k) for 0<=k<=n, generalizes the Motzkin
 *          lattice paths with weights of A003645.
 * https://oeis.org/A201639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangular array read by rows, with entries T(r,c) =
 *            (c+1) * 2^(r-c) * C(2*(r+1), r-c) / (r+1), for 0 ≤ c ≤ r. Here
 *            a(0)=1, a(1)=4, a(2)=1, a(3)=20, …
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 90, u64 n <= 299,
 *            u128 n <= 1080, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

static idx_t _row_from_index(idx_t n)
{
    idx_t r = 0;
    while (1) {
        __uint128_t next = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (next <= n)
            r++;
        else
            break;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Find row r and column c from the linear index n */
    idx_t r = _row_from_index(n);
    __uint128_t r_tri = (__uint128_t)r * (r + 1) / 2;
    idx_t c = n - (idx_t)r_tri;

    /* Compute T(r,c) via the closed form */
    seq_t val = binom(2 * (r + 1), r - c);          /* C(2*(r+1), r-c) */
    val = val * pow2(r - c);                       /* * 2^(r-c) */
    val = val * (seq_t)(c + 1);                    /* * (c+1) */
    val = val / (seq_t)(r + 1);                    /* / (r+1) */

    return val;
}
