/*
 * A051774  Contracted numbers.
 * https://oeis.org/A051774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_contracted
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A051774 – Contracted numbers.
 * A number n is contracted if for its distinct divisors d1<d2<…<dk there exists an i
 * (with 1 ≤ i ≤ k) such that d1+d2+…+d(i‑1) ≥ d(i).  The algorithm below
 * finds the n‑th contracted number by testing every integer in order.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_contracted.h"

/* External function that returns the n-th contracted number. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1; safety */

    uint64_t count = 0;
    uint64_t candidate = 1;

    while (true) {
        if (is_contracted(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }
}
