/*
 * A130466  Triangle where the n-th row consists of the n smallest positive
 *          integers which are coprime to the sum of divisors of n.
 * https://oeis.org/A130466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd, seq_one, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A130466.
   The triangle has row r containing the r smallest positive integers
   coprime to σ(r).  The sequence a(n) is the flattened list of this
   triangle, row by row.  For a given index n (1‑based), we first
   determine the row number r such that T(r−1) < n ≤ T(r) (T(k)=k(k+1)/2),
   then find the (n−T(r−1)−1)‑th coprime integer to σ(r). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Find the row r such that T(r) >= n.
       T(r) = r*(r+1)/2.  Use incremental search – fast enough for n <= 10000. */
    idx_t r = 1;
    idx_t cumulative = 1;            /* T(1) */
    while (cumulative < n) {
        ++r;
        cumulative += r;             /* now cumulative = T(r) */
    }
    /* Start index of row r is cumulative - r + 1. */
    idx_t start_index = cumulative - r + 1;
    idx_t offset = n - start_index; /* 0‑based position within the row */
    /* Compute σ(r). */
    seq_t sigma = sigma_of(r);
    seq_t one = seq_one();

    /* Scan forward from 1, picking numbers coprime to σ(r).
       The offset is small ( < r ), so the loop will finish quickly. */
    seq_t result = 0;
    idx_t candidate = 1;
    idx_t remaining = offset;      /* remaining positions to skip */
    for (;;) {
        seq_t cand_seq = (seq_t)candidate;
        seq_t g = gcd(sigma, cand_seq);
        if (g == one) {
            if (remaining == 0) {
                result = cand_seq;
                break;
            }
            --remaining;
        }
        ++candidate;
    }
    return result;
}
