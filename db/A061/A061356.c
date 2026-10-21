/*
 * A061356  Triangle read by rows: T(n, k) is the number of labeled trees on n
 *          nodes with maximal node degree k (0 < k < n).
 * https://oeis.org/A061356
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 56, u64 n <= 137,
 *            u128 n <= 379, big any n
 * Helpers:   binomial, floor_isqrt_u64, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Definition of OEIS sequence A061356
 * Triangle read by rows: T(n, k) = binomial(n-2, k-1) * (n-1)^(n-k-1)
 * with 0 < k < n.  The flattened sequence a(n) lists the triangle rows
 * one after another, starting with a(2) = 1.
 *
 * This function computes a(n) for any n ≥ 2 by mapping the flat index
 * to its row r and column k, then evaluating T(r, k).
 */

#include "../utils/floor_isqrt_u64.h"   /* floor_isqrt_u64 */
#include "../utils/binomial.h"          /* binomial */
#include "../utils/pow_seq.h"           /* pow_seq */
#include "../utils/seq_mul.h"           /* seq_mul */

seq_t a(idx_t n)
{
    if (n < 2) return 0;              /* values below the offset are undefined */

    /* zero‑based offset within the flattened triangle */
    idx_t s = n - 2;

    /* find the row r that contains this element */
    idx_t discr    = 1 + 8 * s;
    idx_t sqrt_d   = floor_isqrt_u64(discr);
    idx_t r        = (sqrt_d + 1) / 2 + 1;  /* preliminary row estimate */

    /* adjust r in case the floor approximation was not exact */
    idx_t cumulative_before = (r - 2) * (r - 1) / 2;
    if (cumulative_before > s) {
        /* r was too large */
        --r;
        cumulative_before = (r - 2) * (r - 1) / 2;
    } else {
        idx_t cumulative_including = (r - 1) * r / 2;
        if (s >= cumulative_including) {
            /* r was too small */
            ++r;
            cumulative_before = (r - 2) * (r - 1) / 2;
        }
    }

    /* position inside row r (0‑based) */
    idx_t pos = s - cumulative_before;
    idx_t k   = pos + 1;               /* column index: 1 … r‑1 */

    /* compute T(r, k) */
    seq_t bin = binomial(r - 2, k - 1);                 /* binomial part  */
    seq_t pow = pow_seq((seq_t)(r - 1), (int)(r - k - 1));/* power part     */

    return seq_mul(bin, pow);
}
