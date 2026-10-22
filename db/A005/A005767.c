/*
 * A005767  Solutions k to k^2 = a^2 + b^2 + c^2 (a,b,c > 0).
 * https://oeis.org/A005767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer not of the form 2^j or 5*2^j
 *            (Fraser and Gordon); found by counting excluded numbers up to k
 * Range:     u8 n <= 241, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_form, count_valid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_form.h"
#include "../utils/count_valid.h"

seq_t a(idx_t n) {
    seq_t target = (seq_t)n;
    seq_t k = target;
    /* count_valid is nondecreasing; a(n) = n + O(log n), so this is short */
    while (count_valid(k) < target) k++;
    return k;
}
