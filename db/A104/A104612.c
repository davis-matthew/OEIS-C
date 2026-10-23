/*
 * A104612  Write the natural numbers in base 8 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n*(n+1)/2‑th digit of the infinite string obtained by
 *            writing the natural numbers in base‑8 consecutively
 * Range:     u8 n <= 18, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pos = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t remaining = pos;

    seq_t pow = 1;           /* 8^0 */

    for (idx_t d = 1; ; ++d) {
        seq_t start   = pow;             /* 8^(d-1) */
        seq_t next_pow= start * 8;        /* 8^d */
        seq_t count   = next_pow - start;/* numbers with d octal digits */
        seq_t block_digits = count * d;   /* total digits in this block */

        if (remaining > block_digits) {
            remaining -= block_digits;
            pow = next_pow;
            continue;
        }

        seq_t offset_within = (remaining - 1) / d;
        seq_t offset_in_num = (remaining - 1) % d;

        seq_t number = start + offset_within; /* decimal value of the number */

        seq_t divisor = start;
        idx_t offset_idx = (idx_t)offset_in_num;
        for (idx_t i = 0; i < offset_idx; ++i)
            divisor /= 8;

        seq_t digit = (number / divisor) % 8;
        return digit;
    }
}
