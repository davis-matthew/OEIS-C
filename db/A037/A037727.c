/*
 * A037727  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,0,3,1.
 * https://oeis.org/A037727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A037727: a(n) = integer with base‑8 digits given by repeating
   the pattern 2,0,3,1 – take the first n digits of that pattern and interpret
   them as a base‑8 number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case.  The OEIS offset is 1, but we allow n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Number of full 4‑digit blocks and remaining digits. */
    idx_t m = n / 4;      /* number of complete 4‑digit blocks   */
    idx_t r = n % 4;      /* remaining digits after the last block */

    /* Constants: base‑8 value of the 4‑digit block 2031 and 8^4. */
    const seq_t BLOCK   = (seq_t)1049;   /* 2*8³ + 0*8² + 3*8 + 1  */
    const seq_t POW8_4  = (seq_t)4096;   /* 8⁴ */

    /* Build the number by processing each full block first. */
    seq_t val = (seq_t)0;
    for (idx_t i = 0; i < m; ++i)
    {
        val = val * POW8_4 + BLOCK;
    }

    /* Prefix (the last remaining digits).  Determine their base‑8 value. */
    seq_t prefix = (seq_t)0;
    switch (r) {
        case 0: prefix = (seq_t)0;      break;   /* no digits */
        case 1: prefix = (seq_t)2;      break;   /* 2                  */
        case 2: prefix = (seq_t)16;     break;   /* 2 0                */
        case 3: prefix = (seq_t)131;    break;   /* 2 0 3              */
    }

    /* Multiply current value by 8^r to shift the remaining digits to the right
       and add the prefix digits. */
    seq_t pow8_r = (seq_t)1;
    switch (r) {
        case 0: pow8_r = (seq_t)1;      break;
        case 1: pow8_r = (seq_t)8;      break;
        case 2: pow8_r = (seq_t)64;     break;
        case 3: pow8_r = (seq_t)512;    break;
    }

    val = val * pow8_r + prefix;
    return val;
}
