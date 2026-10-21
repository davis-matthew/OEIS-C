/*
 * A062096  a(1) = 2; for n > 1, a(n) is smallest number, greater than a(n-1),
 *          which is relatively prime to the sum of all previous terms.
 * https://oeis.org/A062096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 475, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity_seq, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = smallest integer > a(n-1) that is coprime with the sum of all previous terms
 * a(1) = 2
 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/ensure_capacity_seq.h"

static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static seq_t sum = 0;

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;

    ensure_capacity_seq((size_t)n, &seq_cap, &seq);

    if (seq_len == 0) {
        seq[0] = (seq_t)2;
        sum = seq[0];
        seq_len = 1;
    }

    while (seq_len < (size_t)n) {
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        while (gcd(candidate, sum) != (seq_t)1) candidate += (seq_t)1;
        seq[seq_len] = candidate;
        sum += candidate;
        ++seq_len;
    }

    return seq[(size_t)n - 1];
}
