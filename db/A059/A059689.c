/*
 * A059689  List consisting of all functions from [1,2,3,...,n] into
 *          [1,2,3,...,n] for n=1,2,3,... (written out in full) in
 *          lexicographic order.
 * https://oeis.org/A059689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th term in the concatenation of all finite
 *            sequences of positive integers [1..] enumerated
 *            lexicographically
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1 */

    /* 0‑based target index, in seq_t to allow large arithmetic */
    idx_t target0 = n - 1;
    seq_t target  = (seq_t)target0;

    /* Find the block (function length m) containing target */
    idx_t m = 1;
    seq_t cum = 0;
    while (1) {
        seq_t block = big_pow((seq_t)m, (seq_t)(m + 1)); /* m^(m+1) */
        if (cum + block > target) break;
        cum += block;
        ++m;
    }

    /* Position of the term within the found block */
    seq_t offset_seq = target - cum;                     /* 0‑based within block */
    idx_t offset     = (idx_t)offset_seq;
    idx_t token_index = (idx_t)(offset_seq % m);          /* which entry in the tuple */
    idx_t func_index  = (idx_t)(offset_seq / m);          /* 0‑based function number */

    /* Base‑m representation of func_index: digits are 0…m‑1.
       The most significant digit is position 0. */
    seq_t power = big_pow((seq_t)m, (seq_t)(m - 1));
    seq_t rem   = (seq_t)func_index;        /* small, fits in seq_t */

    for (idx_t pos = 0; pos < m; ++pos) {
        seq_t digit = rem / power;          /* current most significant digit */
        rem %= power;
        if (pos == token_index)
            return digit + 1;               /* map 0→1, 1→2, … */
        if (power > 1) power /= (seq_t)m;
    }

    /* Should never reach here */
    return 0;
}
