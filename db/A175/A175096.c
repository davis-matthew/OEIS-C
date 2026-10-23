/*
 * A175096  Write n in binary (without leading 0's). a(n) = the number of
 *          distinct numerical values made by permutating the runs of 0's and
 *          the runs of 1's, such that the runs (of nonzero length) of 1's
 *          alternate with the runs (of nonzero length) of 0's. The permutated
 *          binary numbers (those not equal to n) m...
 * https://oeis.org/A175096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   cmp_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A175096
   Formula:  Let c1 be the number of runs of 1's and c0 the
   number of runs of 0's in the binary representation of n (without
   leading zeros).  For each multiset of run lengths let
       p = (|multiset|)! / ∏ (frequency_of_each_length)! .
   Then
       a(n) = p1 * p0  if c1 = c0 + 1
              2·p1 * p0  if c1 = c0
   (The factor 2 accounts for the two possible starting bits when the
   numbers of 1- and 0-runs are equal.) */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_size_t.h"

/* Return the number of distinct permutations of a multiset stored
   in arr[0 .. cnt-1] (arr contains the elements, not their
   multiplicities). */
static uint64_t distinct_permutations(size_t cnt, const size_t *arr)
{
    if (cnt <= 1)
        return 1;

    /* factorial of cnt */
    uint64_t perm = 1;
    for (size_t k = 2; k <= cnt; ++k)
        perm *= k;

    /* sort a copy to count multiplicities */
    size_t tmp[64];
    for (size_t i = 0; i < cnt; ++i)
        tmp[i] = arr[i];
    qsort(tmp, cnt, sizeof(size_t), cmp_size_t);

    /* divide by factorial of each multiplicity */
    for (size_t i = 0; i < cnt; ) {
        size_t val = tmp[i];
        size_t freq = 1;
        while (i + 1 < cnt && tmp[i + 1] == val) {
            ++freq;
            ++i;
        }
        for (size_t f = 2; f <= freq; ++f)
            perm /= f;
        ++i;
    }
    return perm;
}

seq_t a(idx_t n)
{
    /* Determine bit length of n (n >= 1). */
    size_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) { ++bitlen; tmp >>= 1; }
    if (bitlen == 0) bitlen = 1;   /* defensive for n == 0, though not used */

    /* Arrays to store run lengths. */
    size_t ones[64], zeros[64];
    size_t c1 = 0, c0 = 0;

    /* Scan bits from most significant to least significant. */
    int cur_bit = 1;               /* first bit is always 1 */
    size_t run_len = 1;

    /* Iterate over remaining bits (pos from bitlen-2 down to 0). */
    for (size_t i = 1; i < bitlen; ++i) {
        int bit = (int)((n >> (bitlen - 1 - i)) & 1);
        if (bit == cur_bit) {
            ++run_len;
        } else {
            if (cur_bit == 1) {
                ones[c1++] = run_len;
            } else {
                zeros[c0++] = run_len;
            }
            cur_bit = bit;
            run_len = 1;
        }
    }

    /* Store the final run. */
    if (cur_bit == 1)
        ones[c1++] = run_len;
    else
        zeros[c0++] = run_len;

    /* Count distinct permutations for each multiset. */
    uint64_t p1 = distinct_permutations(c1, ones);
    uint64_t p0 = distinct_permutations(c0, zeros);

    /* Combine according to the number of runs. */
    uint64_t result = p1 * p0;
    if (c1 == c0)
        result *= 2;

    return (seq_t)result;
}
