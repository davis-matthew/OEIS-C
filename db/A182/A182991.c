/*
 * A182991  Numbers with property that their divisors are odd, even, odd,
 *          even, and so on.
 * https://oeis.org/A182991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_alternating_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute by brute‑force search: a(n) is the n‑th integer whose
   divisors alternate in parity (odd, even, odd, even, ...). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_alternating_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence is defined from n = 1 */

    idx_t found = 0;
    for (uint64_t cand = 1; ; ++cand) {
        if (has_alternating_divisors(cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
    }
}
