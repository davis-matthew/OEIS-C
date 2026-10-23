/*
 * A043572  Numbers whose base-2 representation has exactly 5 runs.
 * https://oeis.org/A043572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043572: numbers whose base‑2 representation has exactly
 * five runs.  The routine generates all binary strings of length L
 * with 5 runs, ordering them lexicographically so that the numeric
 * values are produced in ascending order.  Results are cached in a
 * static array so subsequent calls are O(1). */
#include <stdlib.h>

static seq_t *sequence = NULL;      /* cached sequence values    */
static size_t size     = 0;         /* number of values stored   */
static size_t capacity = 0;         /* allocated capacity         */
static idx_t  current_L = 5;        /* current binary length to
                                      * generate (minimal length is 5)*/

seq_t a(idx_t n)
{
    /* allocate and generate terms until the cache contains at least n */
    while (size < (size_t)n) {
        /* enumerate compositions L = a + b + c + d + e with all parts >= 1 */
        for (idx_t a1 = 1; a1 <= current_L - 4; ++a1) {
            /* b is the run of zeros after the first run of ones.
             * For ascending numeric order, larger b produces smaller
             * numbers, so we iterate b in decreasing order. */
            idx_t max_b = current_L - a1 - 3;
            for (idx_t b1 = max_b; b1 >= 1; --b1) {
                idx_t max_c = current_L - a1 - b1 - 2;
                for (idx_t c1 = 1; c1 <= max_c; ++c1) {
                    /* d is the run of zeros after the third run of ones.
                     * Larger d gives smaller numbers, so iterate d decreasing. */
                    idx_t max_d = current_L - a1 - b1 - c1 - 1;
                    for (idx_t d1 = max_d; d1 >= 1; --d1) {
                        idx_t e1 = current_L - a1 - b1 - c1 - d1; /* >= 1 */

                        /* Build the binary number from the run lengths. */
                        seq_t val = 0;
                        /* first run of 1s */
                        val = (((seq_t)1 << a1) - 1);
                        /* second run of 0s */
                        val <<= b1;
                        /* third run of 1s */
                        val = (val << c1) | (((seq_t)1 << c1) - 1);
                        /* fourth run of 0s */
                        val <<= d1;
                        /* fifth run of 1s */
                        val = (val << e1) | (((seq_t)1 << e1) - 1);

                        /* Ensure capacity and store the value. */
                        if (size == capacity) {
                            size_t new_cap = capacity ? capacity << 1 : 128;
                            seq_t *tmp = (seq_t *)realloc(sequence, new_cap * sizeof(seq_t));
                            sequence = tmp;
                            capacity = new_cap;
                        }
                        sequence[size++] = val;
                    }
                }
            }
        }
        /* move to the next binary length for the next iteration */
        ++current_L;
    }

    return sequence[n - 1];
}
