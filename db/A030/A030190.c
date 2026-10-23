/*
 * A030190  Binary Champernowne sequence (or word): write the numbers
 *          0,1,2,3,4,... in base 2 and juxtapose.
 * https://oeis.org/A030190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf, changed
 * Formula:   a(n) is the nth bit (0‑based) of the binary Champernowne
 *            sequence, obtained by concatenating the binary representations
 *            of 0,1,2,3,…
 * Range:     u8 n <= 2677, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first bit from the number 0  */
    if (n == 0)
        return 0;

    /* remaining bits after the first '0' bit */
    seq_t rem = (seq_t)n;
    rem -= 1;                 /* skip the first bit */

    /* find the binary length 'len' of the block that contains the desired bit */
    idx_t len = 1;
    while (1) {
        seq_t count = ((seq_t)1) << (len - 1);   /* numbers with this length  */
        seq_t bits  = count * len;               /* total bits in this block */

        if (rem < bits)          /* the desired bit is inside this block */
            break;

        rem -= bits;              /* skip this whole block   */
        len++;
    }

    /* offset within this block: which number and which bit of that number */
    idx_t index  = (idx_t)(rem / len);   /* 0‑based index of the number */
    idx_t offset = (idx_t)(rem % len);   /* position inside the number */

    seq_t number = (((seq_t)1) << (len - 1)) + index;
    idx_t shift  = len - 1 - offset;     /* shift to the desired bit */
    seq_t bit    = (number >> shift) & 1;

    return bit;
}
