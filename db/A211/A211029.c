/*
 * A211029  Triangle read by rows in which row n lists the binary words of
 *          length n over the alphabet {1,2} with no initial repeats.
 * https://oeis.org/A211029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 594, u64 n <= 8998,
 *            u128 n <= 8998, big any n
 * Helpers:   bits_to_value, generate_len, generate_row_and_check,
 *            is_valid_partial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generation of A211029: enumerate binary words of length n that have no initial repeated
   block, and return the n-th such word (1‑based) interpreted in digits 1/2. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bits_to_value.h"
#include "../utils/generate_len.h"
#include "../utils/generate_row_and_check.h"
#include "../utils/is_valid_partial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset starts at 1; undefined for n=0 */
    idx_t cumulative = 0;
    for (int len = 1; ; ++len) {
        idx_t targetInRow = (n > cumulative) ? n - cumulative - 1 : (idx_t)-1;
        seq_t ans = 0;
        bool found = false;
        idx_t wordsInRow = generate_row_and_check(len, targetInRow, &ans, &found);
        if (found)
            return ans;
        cumulative += wordsInRow;
    }
}
