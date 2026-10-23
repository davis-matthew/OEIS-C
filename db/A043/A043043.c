/*
 * A043043  Base 10 palindromes that start with 8.
 * https://oeis.org/A043043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 2222, u64 n <= 2222,
 *            u128 n <= 2222, big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the n-th Base‑10 palindrome that starts with 8.
   The palindromes are enumerated in increasing numeric order:
   8, 88, 808, 818, …   
   The algorithm generates the palindrome directly from its position
   without searching or storing a large table.
*/

#include <stdint.h>
#include "../utils/rev_decimal.h"   /* seq_t rev_decimal(seq_t x); */

seq_t a(idx_t n) {
    /* For each length L (>=1) the number of palindromes that start
       with 8 is: 1 for L = 1 or 2; otherwise 10^( (L-1)/2 ).
       We iterate over lengths until the remaining index falls into
       the current block, then construct the palindrome from the
       offset within that block. */

    /* Pre‑compute powers of 10 up to 10^10 – enough for n up to 10000.
       The code uses only exponents ≤ 4, so any overflow in smaller
       integer types is harmless for builds that can represent the
       required values. */
    static unsigned init = 0;
    static seq_t pow10[11];
    if (!init) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 11; ++i)
            pow10[i] = pow10[i-1] * (seq_t)10;
        init = 1;
    }

    idx_t idx = n - 1;            /* 0‑based index */
    /* Length 1 palindrome */
    if (idx == 0)
        return (seq_t)8;
    idx -= 1; /* consume the length‑1 palindrome */

    /* Lengths 2 and greater */
    for (int len = 2; ; ++len) {
        int exp = (len - 1) / 2;        /* exponent for the count */
        seq_t count = pow10[exp];

        if (idx < count) {
            /* Found the correct length */
            int half_len = (len + 1) / 2;          /* number of digits in the first half */
            seq_t prefix_start = (seq_t)8 * pow10[half_len - 1];
            seq_t prefix = prefix_start + (seq_t)idx;

            if (len % 2 == 0) { /* even length L = 2m */
                return prefix * pow10[half_len] + rev_decimal(prefix);
            } else {            /* odd length L = 2m+1 */
                return prefix * pow10[half_len - 1] + rev_decimal(prefix / (seq_t)10);
            }
        } else {
            idx -= (idx_t)count;              /* skip this block */
        }
    }
}
