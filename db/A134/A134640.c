/*
 * A134640  Permutational numbers (numbers with k different digits in
 *          k-positional system).
 * https://oeis.org/A134640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 1215, u32 n <= 5913, u64 n <= 5913,
 *            u128 n <= 5913, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A134640:  Permutational numbers
   For each k = 1,2,3,... the block of values consists of all
   permutations of the digit set {0,1,…,k-1} interpreted as base‑k
   numbers.  The block for a given k has length k! and the numbers are
   listed in ascending order.  The n‑th term a(n) is the
   (n‑1)-st element of the appropriate block.  The function obtains k
   by accumulating factorials until the cumulative sum reaches n,
   then converts the (n‑1)-st rank in that block into the corresponding
   permutation using the factorial number system.  Finally the
   permutation is evaluated in base k.  This algorithm runs in O(k²)
   time with k ≤ 20 for all 64‑bit n.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find k such that sum_{i=1}^{k-1} i! < n <= sum_{i=1}^{k} i! */
    idx_t k = 1;
    idx_t fact = 1;          /* k!  */
    idx_t sum = 0;           /* cumulative sum of factorials up to k */

    for (;; ++k)
    {
        fact *= k;
        sum += fact;
        if (sum >= n) break;
    }

    /* offset of n within the k‑block (0‑indexed) */
    idx_t prev_sum = sum - fact;     /* sum of factorials up to k-1 */
    idx_t offset = (n - prev_sum) - 1;

    /* factorial array fact_arr[i] = i!  for i=0..k */
    idx_t fact_arr[21];
    fact_arr[0] = 1;
    for (idx_t i = 1; i <= k; ++i)
        fact_arr[i] = fact_arr[i-1] * i;

    /* list of remaining digits (always sorted initially) */
    uint8_t remaining[21];
    for (idx_t i = 0; i < k; ++i)
        remaining[i] = (uint8_t)i;

    /* permutation array */
    uint8_t perm[21];
    idx_t rem_cnt = k;

    for (idx_t pos = 0; pos < k; ++pos)
    {
        idx_t f = fact_arr[k-1 - pos];
        idx_t idx = offset / f;
        offset %= f;

        /* select idx-th remaining digit */
        uint8_t chosen = remaining[idx];
        perm[pos] = chosen;

        /* remove chosen digit from remaining list */
        for (idx_t j = idx; j + 1 < rem_cnt; ++j)
            remaining[j] = remaining[j+1];
        --rem_cnt;
    }

    /* compute the numeric value of the permutation in base k */
    seq_t value = 0;
    for (idx_t pos = 0; pos < k; ++pos)
        value = value * (seq_t)k + (seq_t)perm[pos];

    return value;
}
