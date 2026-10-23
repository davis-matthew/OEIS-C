/*
 * A104610  Write the natural numbers in base 6 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the digit at position n*(n+1)/2 in the concatenated
 *            base‑6 expansions of 1,2,3,…
 * Range:     u8 n <= 13, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* position of the desired digit in the infinite digit stream */
    seq_t pos = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Find the block (digit length m) that contains this position */
    idx_t  m    = 1;          /* current digit length */
    seq_t  pow6 = 1;          /* 6^(m‑1) – first number of this block */

    while (1) {
        seq_t count_nums  = 5 * pow6;            /* numbers in this block */
        seq_t digits_block = (seq_t)m * count_nums; /* digits in this block */

        if (pos <= digits_block)
            break;                            /* pos lies inside this block */

        pos -= digits_block;                  /* skip this block */
        pow6 *= 6;                             /* advance to next power */
        ++m;
    }

    /* Index of the digit within the number */
    idx_t digit_index  = (idx_t)((pos - 1) % m);
    idx_t num_index    = (idx_t)((pos - 1) / m);
    seq_t number       = pow6 + num_index;    /* the number whose digit we need */

    /* Extract the digit from the base‑6 representation */
    idx_t steps = m - 1 - digit_index;        /* how many divisions by 6 */
    seq_t tmp = number;
    for (idx_t i = 0; i < steps; ++i)
        tmp /= 6;
    return tmp % 6;                          /* the desired digit (0‑5) */
}
