/*
 * A234579  Let S_n = 0 followed by base-4 expansion of n, reversed; sequence
 *          is concatenation of S_0, S_1, S_2, ...
 * https://oeis.org/A234579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9594, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A234579
   Definition – the infinite string is obtained by concatenating, for k = 0, 1, 2, …,
   the string S_k, where S_k consists of a leading '0' followed by the base‑4
   expansion of k read backwards.  The sequence term a(n) is the nth digit
   (0‑based) of that string.  For example
         S_0 = "00",  S_1 = "01",  S_2 = "02",  S_4 = "001", …
*/

#include <stdint.h>

/* Return the nth term of the sequence.  idx_t and seq_t are defined by the
   test harness. */
seq_t a(idx_t n)
{
    /* The first two digits come from S_0.  They are always 0. */
    if (n < 2) return (seq_t)0;

    idx_t pos = n - 2;          /* position in the concatenation of S_1, S_2, … */

    /* Find the block of numbers that have the same number of base‑4 digits. */
    unsigned d = 1;             /* current number of base‑4 digits */
    for (;;) {
        /* numbers with d digits: from 4^(d-1) to 4^d - 1 */
        idx_t pow4 = (idx_t)1 << (2 * (d - 1));          /* 4^(d-1) */
        idx_t count = (idx_t)3 * pow4;                  /* 3*4^(d-1) numbers */
        idx_t block_len = count * (d + 1);              /* length of S_k for all these numbers */

        if (pos < block_len) break;                    /* our digit lies here */
        pos -= block_len;
        d++;
    }

    /* Compute which specific number within the block and the offset inside its S_k. */
    idx_t item   = pos / (d + 1);               /* index (0‑based) inside the block */
    idx_t offset = pos % (d + 1);               /* position inside S_k */

    if (offset == 0) return (seq_t)0;           /* the leading zero of S_k */

    /* The digit at offset > 0 equals the (offset‑1)-th least significant base‑4 digit. */
    idx_t number = ((idx_t)1 << (2 * (d - 1))) + item;  /* the actual integer k */

    unsigned shift = 2 * (offset - 1);          /* position of the required digit in bits */
    seq_t digit = (seq_t)((number >> shift) & 3ULL);

    return digit;
}
