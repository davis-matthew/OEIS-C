/*
 * A143518  Triangle read by rows, A054525 * (A010051 * 0^(n-k)), 1<=k<=n.
 * https://oeis.org/A143518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = μ(r/k) if k is prime and k | r, where r = ceil((√(1+8n) –
 *            1)/2) is the row index of n and k = n – r(r–1)/2 is the column
 *            index. Otherwise a(n) = 0
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   _floor_isqrt_u64, is_prime, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes only the utilities required to compute the formula. */
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime.h"
#include "../utils/mobius.h"

/* Return the n-th term of OEIS sequence A143518. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* guard for n < 1 */

    /* Compute row index r.  r = ceil((-1 + sqrt(1+8n))/2). */
    idx_t t   = 1 + (idx_t)8 * n;         /* 1 + 8n */
    idx_t s   = _floor_isqrt_u64(t);      /* floor sqrt(1+8n) */
    idx_t r   = (s - 1) / 2;              /* floor part of the root */
    if (r * (r + 1) / 2 < n) r++;         /* adjust to ceil */

    /* Compute column index k (1‑based). */
    idx_t prev_sum = r * (r - 1) / 2;     /* sum of lengths of previous rows */
    idx_t k = n - prev_sum;               /* column position in the current row */

    /* If k is not prime, the value is 0. */
    if (!is_prime(k))
        return (seq_t)0;

    /* If k does not divide r, the entry of the triangle is 0. */
    if (r % k != 0)
        return (seq_t)0;

    /* Otherwise return μ(r/k). */
    idx_t m = r / k;
    return mobius(m);
}
