/*
 * A146755  Alliterative numbers: Positive integers n such that all words in
 *          n's name begin with the same letter; ignore numbers with one-word
 *          names.
 * https://oeis.org/A146755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   cmp_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Enumerate all two‑digit alliterative numbers and all
   numbers of the form A thou­sand + B, where A is a number whose
   English name starts with “t” (10,20,30,2,3,12,13) and B is a
   “t‑start” number less than 100.  Return the n‑th smallest such
   number.  The maximum term is 30 033 (30 000 + 33).  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_arr.h"

/* Static buffer for all terms (max 98 items). */
static seq_t seq_values[200];
static size_t seq_len = 0;
static bool initialized = false;

seq_t a(idx_t n)
{
    if (!initialized) {
        size_t idx = 0;

        /* Two‑digit alliterative numbers (14 terms). */
        static const seq_t two_digit[14] = {
            22, 23, 32, 33, 44, 45,
            54, 55, 66, 67, 76, 77,
            88, 99
        };
        for (size_t i = 0; i < sizeof(two_digit)/sizeof(two_digit[0]); ++i)
            seq_values[idx++] = two_digit[i];

        /* Base numbers starting with 't' (36‑140). */
        static const seq_t base_thousands[7] = {
            10, 20, 30, 2, 3, 12, 13
        };

        /* All “t‑start” numbers less than 100 (11 terms). */
        static const seq_t t_start[11] = {
            2, 3, 10, 12, 13, 20, 30,
            22, 23, 32, 33
        };

        for (size_t i = 0; i < sizeof(base_thousands)/sizeof(base_thousands[0]); ++i) {
            seq_t base = base_thousands[i];
            seq_t thousand = base * (seq_t)1000;

            /* Pure thousand number. */
            seq_values[idx++] = thousand;

            /* Add all B variants (B<100). */
            for (size_t j = 0; j < sizeof(t_start)/sizeof(t_start[0]); ++j)
                seq_values[idx++] = thousand + t_start[j];
        }

        /* Sort the unique terms. */
        qsort(seq_values, idx, sizeof(seq_t), cmp_seq_arr);
        seq_len = idx;
        initialized = true;
    }

    if (n == 0 || (idx_t)seq_len < n)
        return 0;
    return seq_values[(size_t)n - 1];
}
