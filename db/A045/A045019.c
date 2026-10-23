/*
 * A045019  Numbers whose base-4 representation contains exactly one 0 and one
 *          1.
 * https://oeis.org/A045019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑4 representation
 *            contains exactly one digit 0 and exactly one digit 1
 * Range:     u8 n <= 45, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   has_one_zero_one_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: straightforward brute‑force search. */

#include <stdbool.h>
#include "../utils/has_one_zero_one_one.h"

/* Sequence entry function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n < 1, return 0 for safety */
    idx_t count = 0;
    idx_t x = 1;
    while (1) {
        if (has_one_zero_one_one(x)) {
            ++count;
            if (count == n) {
                return (seq_t)x;
            }
        }
        ++x;
    }
}
