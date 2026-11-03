/*
 * A044616  Numbers n such that string 6,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for numbers whose base‑8 representation
   contains the substring 60 but does not contain it after adding 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper that checks whether a number satisfies the condition. */
static bool has_valid_60(seq_t value)
{
    /* an array big enough for all numbers that occur in the tests */
    static uint8_t digits[256];
    size_t len = 0;
    seq_t tmp = value;

    if (tmp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (tmp != 0) {
            digits[len++] = (uint8_t)(tmp % 8);
            tmp     /= 8;
        }
    }

    int total_60   = 0;
    int resolved_60 = 0;

    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == 6 && digits[i - 1] == 0) {
            ++total_60;
            /* check that all digits below the 0 are 7 */
            bool all_lower_sevens = true;
            for (size_t j = 0; j + 1 < i; ++j) {
                if (digits[j] != 7) {
                    all_lower_sevens = false;
                    break;
                }
            }
            if (all_lower_sevens) {
                ++resolved_60;
            }
        }
    }
    return (total_60 == 1 && resolved_60 == 1);
}

seq_t a(idx_t n)
{
    seq_t candidate = 0;
    idx_t count    = 0;

    while (1) {
        ++candidate;
        if (has_valid_60(candidate)) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
    }
}
