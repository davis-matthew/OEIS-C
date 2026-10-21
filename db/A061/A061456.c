/*
 * A061456  Binary representation of a(n) corresponds to "ana"-sequence.
 * https://oeis.org/A061456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   expand_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is obtained by starting with the ternary string “1” and
 * iteratively replacing every 1 by “121” and every 2 by “122”.  
 * After (n‑2) substitutions (n≥2) the resulting ternary string is
 * encoded into binary by mapping 1 → 101 (5) and 2 → 100 (4).  
 * The resulting binary number is a(n).  a(1) is defined as 1.
 */
#include <stdbool.h>
#include "../utils/expand_digit.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t val = (seq_t)0;
    /* depth = n-2 substitutions starting from a single digit “1” */
    expand_digit(&val, (int)(n - 2), 1);
    return val;
}
