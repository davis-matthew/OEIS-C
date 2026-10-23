/*
 * A136081  Array, read by antidiagonals, numbers with an odd number of 1's in
 *          base k expansion.
 * https://oeis.org/A136081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   count_odd, ith_odd_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A136081
 *
 * a(n) is the entry obtained by reading the array A(k,n) by
 * antidiagonals, where A(k,n) is the n‑th number having an odd
 * number of 1’s in its base‑k expansion.  k ≥ 1.
 * The function accepts an index n (1‑based) and returns the
 * corresponding sequence element.
 *
 * The implementation proceeds as follows:
 *
 *   1. Determine the diagonal s (k + n) for which the n‑th
 *      entry lies on.  The diagonal lengths are 1, 2, 3, …,
 *      so the cumulative count up to diagonal s is (s‑1)s/2.
 *
 *   2. Find the pair (k, m) on that diagonal:
 *         k = (s‑1) – offset,
 *         m = offset + 1,
 *      where “offset” is the zero‑based position inside the
 *      diagonal.
 *
 *   3. Compute the m‑th integer having an odd number of
 *      1’s in base k.  For k = 1 the answer is simply 2m – 1.
 *
 *      For k ≥ 2 this is done by binary searching the smallest
 *      number x such that the count of integers ≤ x with an odd
 *      number of 1’s in base k is at least m.  The count is
 *      evaluated by a digit DP that works directly on the
 *      digits of x in base k, using only addition, subtraction,
 *      multiplication and comparison.
 *
 *   4. The answer is returned as a value of type seq_t.
 *
 * All calculations that might overflow 64‑bit quantities use
 * unsigned __int128 for safety.  The DP is capped at the
 * required target value, which is at most 141 for the range of
 * indices that the problem tests.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/count_odd.h"
#include "../utils/ith_odd_number.h"

/* Forward declaration of the required sequence type    */
extern seq_t a(idx_t n);

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */

/* ---------------------------------------------------------- */
/* Main sequence function: a(n)
 * ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Determine the diagonal s such that (s-1)s/2 >= n   */
    double dn = (double)n;
    uint64_t s = (uint64_t)floor((sqrt(1.0 + 8.0 * dn) + 1.0) / 2.0);
    while ((unsigned __int128)(s - 1) * s / 2 < n) ++s;
    while (s > 2 && (unsigned __int128)(s - 2) * (s - 1) / 2 >= n) --s;

    /* Count of entries before this diagonal */
    unsigned __int128 prev_cnt =
        (unsigned __int128)(s - 2) * (s - 1) / 2;
    unsigned __int128 diff = (unsigned __int128)n - prev_cnt - 1;
    idx_t offset = (idx_t)diff;               /* zero‑based position in diagonal */

    idx_t k = (s - 1) - offset;   /* base */
    idx_t m = offset + 1;         /* n‑th item on that row */

    return ith_odd_number((uint64_t)k, m);
}
