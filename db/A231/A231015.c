/*
 * A231015  Least k such that n = +- 1^2 +- 2^2 +- 3^2 +- 4^2 +- ... +- k^2
 *          for some choice of +- signs.
 * https://oeis.org/A231015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 9960, u16 n <= 9960, u32 n <= 9960, u64 n <= 9960,
 *            u128 n <= 9960, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A231015
 * a(n) = least k such that n = ±1² ± 2² ± … ± k²
 *
 * The algorithm builds the set of reachable sums using a dynamic bitset.
 * After processing the first k−1 squares, the set of reachable sums is
 * stored in a bit array of length 2·S+1, where S is the sum of the
 * squares up to k−1.  Adding the k‑th square creates a new bit array of
 * length 2·(S+k²)+1: the old sums are kept for the “−” sign, and every
 * old sum shifted by 2·k² is marked for the “+” sign.  The first time the
 * target value n falls within the current range and its corresponding
 * bit is set, the current k is the required minimal value.
 *
 * The routine works for any seq_t satisfying the arithmetic operations
 * listed in the requirements; it uses only integer arithmetic and
 * dynamic allocation of a uint8_t bit array.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* target as signed 64‑bit for offset calculations */
    int64_t target = (int64_t)n;

    /* bitset for reachable sums, index = sum + S */
    uint8_t *reach = (uint8_t*)malloc(1);
    if (!reach) exit(EXIT_FAILURE);
    reach[0] = 1;          /* sum 0 reachable with no squares   */
    size_t len = 1;        /* current length of reach array     */
    uint64_t S = 0;        /* sum of squares processed so far   */

    for (uint64_t k = 1; ; ++k) {
        uint64_t sq = k * k;          /* k²  */
        S += sq;                      /* new total sum of squares  */
        size_t new_len = 2 * (size_t)S + 1;
        uint8_t *new_reach = (uint8_t*)calloc(new_len, 1);
        if (!new_reach) {
            free(reach);
            exit(EXIT_FAILURE);
        }
        size_t shift = 2 * sq;        /* shift for the “+” sign       */

        /* propagate old sums to the new array */
        for (size_t i = 0; i < len; ++i) {
            if (reach[i]) {
                new_reach[i] = 1;               /* “−” sign  */
                size_t j = i + shift;
                new_reach[j] = 1;               /* “+” sign  */
            }
        }
        free(reach);
        reach = new_reach;
        len    = new_len;

        /* does current k allow the target? */
        int64_t idx = target + (int64_t)S;    /* index in new_reach   */
        if (idx >= 0 && idx < (int64_t)len && reach[idx]) {
            seq_t res = (seq_t)k;
            free(reach);
            return res;
        }
    }
}
