/*
 * A102660  List of Lyndon words on {1,2,3} sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A102660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th Lyndon word over the alphabet {1,2,3} sorted
 *            first by length and then lexicographically. The word is returned
 *            as a decimal integer whose digits are 1, 2, 3
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 9382, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gen, is_lyndon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gen.h"
#include "../utils/is_lyndon.h"

/* ------------------------------------------------------------------ */
/* Helper: test whether the word stored in 'd[0..L-1]' is a Lyndon word */

/* ------------------------------------------------------------------ */
/* Recursive generator of all words of a fixed length L
   The words are explored in lexicographic order.
   When the global counter reaches the target rank, the current
   word (as a decimal integer) is stored in *result. */

/* ------------------------------------------------------------------ */
/* Main sequence function */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t global_count = 0;
    seq_t result = 0;
    int digits[20];               /* maximum length needed for n up to 10000 */

    for (idx_t L = 1; ; ++L)
    {
        if (gen(0, L, 0, digits, &global_count, n, &result))
            return result;
    }
}
