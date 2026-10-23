/*
 * A032899  Numbers whose base-2 representation Sum_{i=0..m} d(i)*2^i has d(0)
 *          <= d(1) >= d(2) <= ...
 * https://oeis.org/A032899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032899: the nth number whose binary digits d(i)
   satisfy d(0) <= d(1) >= d(2) <= d(3) >= ... (computed by
   brute‑force search). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Check if the binary representation of x satisfies the alternating
   inequalities defined in the description.  */
static bool valid(seq_t x)
{
    /* Compute the number of bits in x. */
    size_t bitlen = 0;
    seq_t t = x;
    while (t) {
        ++bitlen;
        t >>= 1;
    }

    /* A single-bit number always satisfies the condition. */
    if (bitlen <= 1) {
        return true;
    }

    /* Verify the inequality for each adjacent pair of bits. */
    for (size_t i = 0; i + 1 < bitlen; ++i) {
        unsigned int bit_i   = (unsigned int)((x >> i) & 1);
        unsigned int bit_ip1 = (unsigned int)((x >> (i + 1)) & 1);
        if ((i & 1) == 0) {          /* i even:  d(i) <= d(i+1) */
            if (bit_i > bit_ip1) return false;
        } else {                     /* i odd:   d(i) >= d(i+1) */
            if (bit_i < bit_ip1) return false;
        }
    }
    return true;
}

/* Return the nth member of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    seq_t val = 0;          /* start before the first candidate */
    idx_t  count = 0;       /* number of valid values found so far */

    while (count < n) {
        ++val;                      /* next integer to test */
        if (valid(val)) {
            ++count;
        }
    }
    return val;
}
