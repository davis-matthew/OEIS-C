/*
 * A037194  Number of vowels in n (in French).
 * https://oeis.org/A037194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = number of vowels in the French word for n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   count_french
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for the standard integer types that the build may use */
#include "../utils/count_french.h"

seq_t a(idx_t n)
{
    return count_french(n);
}
