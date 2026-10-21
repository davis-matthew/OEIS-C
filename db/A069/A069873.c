/*
 * A069873  Smallest concatenation of powers of 2 in ascending order that is a
 *          multiple of n, or 0 if no such number exists.
 * https://oeis.org/A069873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest concatenation of the powers of 2 *
 *            1,2,4,8,16,32,… that is a multiple of n; 0 if no such number
 *            exists * (for all n divisible by 5)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* For n ≡ 0 (mod 5) no concatenation can be divisible by n. */
    if (n % 5 == 0)
        return 0;

    seq_t val = 0;           /* current concatenated number */
    seq_t pow2 = 1;          /* current power of 2 (2^k) */
    seq_t pow10_big = 1;     /* 10^d for the current number of digits d */
    idx_t pow10_mod = 1 % n; /* pow10_big modulo n */
    idx_t pow2_mod = 1 % n;  /* current pow2 modulo n */
    idx_t rem = 0;           /* value of val modulo n */
    int current_d = 0;       /* number of digits in pow10_big */

    for (;;) {
        /* Number of decimal digits of the current power of 2. */
        int pow2_digits = digit_len(pow2);

        /* Extend pow10_big (and its modulus) until it matches pow2_digits. */
        while (current_d < pow2_digits) {
            pow10_big *= 10;
            pow10_mod = (pow10_mod * 10) % n;
            current_d++;
        }

        /* Compute the new remainder if we append this pow2. */
        idx_t new_rem = ((rem * pow10_mod) + pow2_mod) % n;

        /* Update the concatenated value. */
        val = val * pow10_big + pow2;

        /* Return if the number is a multiple of n. */
        if (new_rem == 0)
            return val;

        /* Prepare for the next power of 2. */
        pow2 = pow2 * 2;
        pow2_mod = (pow2_mod * 2) % n;
        rem = new_rem;
    }
}
