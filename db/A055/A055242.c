/*
 * A055242  Largest base in which n is not divisible by any of its digits (0
 *          if no such base).
 * https://oeis.org/A055242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: for a given n compute the largest base b (2 ≤ b ≤ n − 1)
   such that no digit of n in base b is zero and n is not divisible
   by any of those digits.  Return that b, or 0 if no such base exists. */

#include <stdbool.h>

/* n and the result are of type idx_t (uint64_t) and seq_t
   (varies with the build).  All arithmetic is performed using only
   +, -, *, /, %, comparisons and the usual integer constants. */
seq_t a(idx_t n)
{
    /* For n < 2 no base in the required range exists. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Convert the integer index once; it is used in every digit test. */
    seq_t n_val = (seq_t)n;

    /* Search from the largest admissible base downwards. */
    for (idx_t base = n - 1; base >= 2; --base) {
        seq_t temp   = n_val;
        seq_t b_val  = (seq_t)base;
        bool ok = true;

        /* Examine every digit of n in base b. */
        while (temp > (seq_t)0 && ok) {
            seq_t digit = temp % b_val;
            if (digit == (seq_t)0 || (n_val % digit) == (seq_t)0) {
                ok = false;
                break;
            }
            temp = temp / b_val;
        }

        if (ok) {
            /* Found the largest base satisfying the condition. */
            return (seq_t)base;
        }
    }

    /* No suitable base was found. */
    return (seq_t)0;
}
