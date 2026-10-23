/*
 * A115810  Sequence A115809 in binary.
 * https://oeis.org/A115810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115809 in binary, expressed as the decimal integer whose digit
   pattern matches the binary string described below.
   Formula: a(0) = 0.
   For n ≥ 1:
     let t be the smallest integer with (t+1)(t+2)/2 ≥ n
     (t is the total number of bits after the initial five 1’s).
     let offset = n – t(t+1)/2 – 1   (0‑based index inside the t‑block)
     let k = offset,  l = t – k.
     a(n) = decimal integer represented by the bit string
            11111 followed by k ones and then l zeros.
   The function builds this bit string and converts it to a seq_t value.
*/

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* find block index t and offset inside the block */
    idx_t t = 0;
    idx_t remaining = n;
    while (remaining > t + 1) {
        remaining -= (t + 1);
        ++t;
    }
    idx_t offset = remaining - 1;          /* 0‑based offset */
    idx_t k = offset;
    idx_t l = t - k;                       /* zeros after the k ones */

    /* build the bit pattern: 11111 + k×'1' + l×'0' */
    idx_t len = 5 + t;                     /* total number of bits */
    char *bits = (char *)malloc(len + 1);
    if (!bits) return 0;                   /* out of memory → return 0 */
    idx_t pos = 0;
    /* five leading ones */
    for (idx_t i = 0; i < 5; ++i) bits[pos++] = '1';
    /* k additional ones */
    for (idx_t i = 0; i < k; ++i) bits[pos++] = '1';
    /* l zeros */
    for (idx_t i = 0; i < l; ++i) bits[pos++] = '0';
    bits[pos] = '\0';                     /* NUL terminator */

    seq_t result = seq_from_str(bits);
    free(bits);
    return result;
}
