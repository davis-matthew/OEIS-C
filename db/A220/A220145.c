/*
 * A220145  The Collatz (3x+1) iteration mod 2 with bits combined.
 * https://oeis.org/A220145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A220145 – reverse parity bits of the Collatz sequence
   as a decimal number.
   For n ≥ 1, let the Collatz trajectory starting from n be
   n₀ = n, n₁, n₂, … , n_k = 1.
   Record the parity bit of each n_i (odd → 1, even → 0).
   Reverse this bit string and interpret it as a decimal integer
   whose digits are the bits.  That decimal integer is a(n).
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset 1 – undefined for n=0 */

    /* collect parity bits of the Collatz iteration */
    size_t cap  = 16;
    uint8_t *bits = (uint8_t *)malloc(cap);
    if (!bits)
        return (seq_t)0;

    idx_t cur = n;
    size_t len = 0;

    while (1) {
        uint8_t b = (uint8_t)(cur & 1);      /* 1 if odd, 0 if even */

        if (len == cap) {                   /* grow array if needed */
            size_t new_cap = cap << 1;
            uint8_t *tmp = (uint8_t *)realloc(bits, new_cap);
            if (!tmp) {
                free(bits);
                return (seq_t)0;
            }
            bits = tmp;
            cap = new_cap;
        }
        bits[len++] = b;

        if (cur == 1)
            break;

        if ((cur & 1) == 0)
            cur >>= 1;                      /* even case */
        else
            cur = 3 * cur + 1;              /* odd case */
    }

    /* build the answer: reverse the bit string and treat it as decimal digits */
    seq_t ans = 0;
    for (size_t i = len; i > 0; ) {
        --i;
        ans = ans * 10 + bits[i];
    }

    free(bits);
    return ans;
}
