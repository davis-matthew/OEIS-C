/*
 * A210666  Numbers with at least three digits in which all digits but one are
 *          the same.
 * https://oeis.org/A210666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 802, u32 n <= 3678, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_str, generate_block, parse_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A210666.
   Each term is the smallest integer having at least 3 digits,
   where exactly one digit is distinct and all others are equal.
   The function enumerates all k‑digit numbers with this property,
   sorts them in ascending order, and returns the n‑th term. */

#include <stdlib.h>
#include <string.h>
#include "../utils/compare_str.h"
#include "../utils/generate_block.h"
#include "../utils/parse_str.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    idx_t cumulative = 0;
    int k = 3;

    while (1) {
        char **vec = NULL;
        size_t cnt = 0;
        generate_block(k, &vec, &cnt);

        if (cumulative + (idx_t)cnt >= n) {
            /* Desired block found: sort and pick the element. */
            qsort(vec, cnt, sizeof(char *), compare_str);
            size_t idx = (size_t)(n - cumulative - 1);
            seq_t result = parse_str(vec[idx]);

            /* Free memory before returning. */
            for (size_t i = 0; i < cnt; ++i) free(vec[i]);
            free(vec);
            return result;
        }

        /* Not in this block: free and continue to next k. */
        for (size_t i = 0; i < cnt; ++i) free(vec[i]);
        free(vec);
        cumulative += (idx_t)cnt;
        ++k;
    }
}
