/*
 * A080645  a(1) = 1; for n>1, a(n) is taken to be the smallest integer
 *          greater than a(n-1) which is consistent with the condition "for
 *          n>1, if n is a member of the sequence then a(n) is even".
 * https://oeis.org/A080645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(3)=4. For k≥1, abs(j)≤2^k: a(3·2^k + j) =
 *            4·2^k + (3·j + |j|)/2 (all operations integer‑valued, division
 *            truncates toward zero)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range calls */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* Determine k such that n ∈ [2·2^k, 4·2^k]  (k ≥ 1) */
    idx_t temp = n;
    idx_t msb = 0;                 /* floor(log2(n)) */
    while (temp >>= 1) ++msb;
    idx_t k = msb - 1;             /* k ≥ 1 for n ≥ 4 */

    idx_t pow2k = (idx_t)1 << k;   /* 2^k */

    seq_t base = ((seq_t)4) * (seq_t)pow2k;

    /* j = n – 3·2^k may be negative */
    int64_t j = (int64_t)n - (int64_t)3 * (int64_t)pow2k;
    int64_t abs_j = j >= 0 ? j : -j;
    int64_t numer = 3 * j + abs_j;        /* 3·j + |j| */

    seq_t result = base + (seq_t)(numer / 2);   /* integer division truncates toward zero */
    return result;
}
