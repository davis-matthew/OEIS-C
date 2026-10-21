/*
 * A223569  T(n,k)=Number of nXk 0..1 arrays with antidiagonals unimodal.
 * https://oeis.org/A223569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 69, u64 n <= 159,
 *            u128 n <= 389, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A223569
   T(n,k) = ∏_{i=1}^{min-1} (1+i(i+1)/2)^2  ·  (1+min(min+1)/2)^{max-min+1}
   The sequence a(n) is obtained by traversing T(i,j) in
   anti‑diagonal order: (1,1), (1,2),(2,1), (1,3),(2,2),(3,1),…
   The nth term corresponds to the nth anti‑diagonal element.
*/

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;            /* highest power of four ≤ 2^64 */
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else
            r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* Fast exponentiation of a seq_t base by a idx_t exponent. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1U) result *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return result;
}

/* Return the nth term of OEIS A223569.  The index n starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* guard for accidental 0 */

    /* --- Locate the anti‑diagonal that contains the nth element --- */
    /* Find the smallest integer s with s*(s+1)/2 ≥ n. */
    idx_t d  = 1 + 8 * n;
    idx_t sq = isqrt_u64(d);
    idx_t s  = (sq - 1) / 2;                    /* floor of (-1+sqrt(1+8n))/2 */
    if (s * (s + 1) / 2 < n) s++;               /* correct to ceiling */

    /* Position within diagonal s (0‑based). */
    idx_t prev    = s * (s - 1) / 2;            /* elements before this diagonal */
    idx_t offset  = n - prev - 1;              /* 0‑based offset on diagonal */

    /* Coordinates (i,j) in the table: i = offset+1, j = s-offset */
    idx_t i = offset + 1;
    idx_t j = s - offset;

    idx_t min_ij = (i < j) ? i : j;
    idx_t max_ij = (i > j) ? i : j;

    seq_t result = 1;

    /* Product over t = 1 .. min_ij-1 of (1 + t(t+1)/2)^2 */
    for (idx_t t = 1; t < min_ij; ++t) {
        seq_t base = 1 + (seq_t)t * (t + 1) / 2;
        result *= base;
        result *= base;                      /* square the base */
    }

    /* Final factor: (1 + min_ij(min_ij+1)/2)^(max_ij - min_ij + 1) */
    seq_t base2 = 1 + (seq_t)min_ij * (min_ij + 1) / 2;
    idx_t exp1  = max_ij - min_ij + 1;
    result *= pow_seq(base2, exp1);

    return result;
}
