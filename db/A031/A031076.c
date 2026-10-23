/*
 * A031076  Write n in base 9 and juxtapose.
 * https://oeis.org/A031076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 614, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th digit of the base‑9 Champernowne constant (concatenation of
   base‑9 representations of the positive integers). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, harmless */

    seq_t rem = (seq_t) n;        /* remaining digits to skip, as seq_t for safe compare */
    seq_t pow9 = 1;               /* 9^(L-1) : leading power for current block (L digits) */
    idx_t  L   = 1;               /* current number of digits of block */

    for ( ; ; ++L) {
        /* digits contributed by all L‑digit numbers in this block:
           8 * 9^(L-1) numbers, each with L digits */
        seq_t block_digits = (seq_t)8 * pow9 * (seq_t)L;

        if (rem > block_digits) {
            rem -= block_digits;
            pow9 *= (seq_t)9;    /* move to next block (increase digit length) */
        } else {
            /* We are inside the block that contains the target digit. */
            idx_t pos = (idx_t)(rem - 1);          /* 0‑based position in the block */
            idx_t number_index = pos / L;          /* which number in the block (0‑based) */
            idx_t digit_pos    = pos % L;          /* position inside that number (0‑most significant) */

            seq_t number = pow9 + (seq_t)number_index; /* actual integer whose digits we need */

            /* The weight of the desired digit (most significant == 0). */
            seq_t denom = pow9;
            for (idx_t i = 0; i < digit_pos; ++i) {
                denom /= (seq_t)9;   /* divide by 9 for each step towards the least significant digit */
            }

            seq_t digit = (number / denom) % (seq_t)9; /* extract the digit */
            return digit;
        }
    }
}
