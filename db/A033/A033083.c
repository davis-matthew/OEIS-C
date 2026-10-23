/*
 * A033083  Numbers in which all pairs of consecutive base-5 digits differ by
 *          2.
 * https://oeis.org/A033083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 57, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gen_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A033083: numbers whose base‑5 digits differ by 2.
   The routine enumerates valid base‑5 strings of increasing length,
   uses a small DP to skip whole lengths, and performs a depth‑first
   generation of the wanted length in increasing numeric order. */

#include <stdbool.h>
#include "../utils/gen_len.h"

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n <= 4) return (seq_t)n;

    /* dp[ d ] = number of valid strings of current length that end with digit d */
    idx_t prev[5] = {0, 1, 1, 1, 1};
    idx_t curr[5], next;
    idx_t len = 1;          /* current length being examined */
    idx_t cumulative = 0;   /* total count of all shorter lengths */

    idx_t target_len = 0;
    idx_t target_index = 0;

    /* Find the length that contains the n-th number. */
    while (1) {
        /* Count how many strings of this length exist.
           For length 1, prev[0]==0, so summing all entries works. */
        idx_t count_len = 0;
        for (int d = 0; d < 5; ++d) count_len += prev[d];

        if (cumulative + count_len >= n) {
            target_len = len;
            target_index = n - cumulative;
            break;
        }

        cumulative += count_len;

        /* Advance DP to the next length. */
        for (int d = 0; d < 5; ++d) {
            next = 0;
            if (d - 2 >= 0) next += prev[d - 2];
            if (d + 2 <= 4) next += prev[d + 2];
            curr[d] = next;
        }
        for (int d = 0; d < 5; ++d) prev[d] = curr[d];
        ++len;
    }

    /* For length 1 the answer is trivial. */
    if (target_len == 1) return (seq_t)target_index;

    /* Generate numbers of the target length in numeric order. */
    seq_t result = 0;
    idx_t local_cnt = 0;

    for (int first = 1; first <= 4; ++first) {
        if (gen_len(target_len - 1, first, (seq_t)first,
                    &local_cnt, target_index, &result))
            return result;
    }

    /* Control should never reach here. */
    return result;
}
