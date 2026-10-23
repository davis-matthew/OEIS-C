/*
 * A104607  Write the natural numbers in base 3 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A104607:
   a(n) is the last digit of the concatenation of the base‑3
   representations of the natural numbers 1,2,3,…, at position
   n*(n+1)/2.  The digits are read row‑by‑row with 1 digit in row 1,
   2 digits in row 2, etc.  The last digit of each row yields the sequence. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Position in the infinite digit stream */
    seq_t pos = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t remaining = pos;

    /* Current block: numbers with 'len' base‑3 digits */
    idx_t len = 1;
    seq_t pow3prev = 1;   /* 3^(len-1) */
    seq_t pow3 = 3;       /* 3^len   */

    while (1) {
        seq_t count_in_len = pow3 - pow3prev;          /* numbers of this length */
        seq_t digits_this_len = count_in_len * (seq_t)len;

        if (remaining > digits_this_len) {
            /* Skip the whole block */
            remaining -= digits_this_len;
            ++len;
            pow3prev = pow3;
            pow3 *= 3;
            continue;
        }

        /* The desired digit lies in this block */
        seq_t num_index   = (remaining - 1) / (seq_t)len;   /* 0‑based index of the number within the block */
        seq_t digit_index = (remaining - 1) % (seq_t)len;   /* 0‑based position inside the number */
        seq_t number      = pow3prev + num_index;

        /* Extract the digit at position digit_index from left to right */
        seq_t power_of_3 = pow3 / 3;                    /* 3^(len-1) */
        idx_t target = (idx_t)digit_index;

        for (idx_t i = 0; i < len; ++i) {
            seq_t digit = (number / power_of_3) % 3;
            if ((idx_t)i == target)
                return digit;
            number %= power_of_3;
            power_of_3 /= 3;
        }
        /* Should never reach here */
        return 0;
    }
}
