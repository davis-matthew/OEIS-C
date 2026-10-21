/*
 * A053069  Numbers that in Italian begin with the letter "d".
 * https://oeis.org/A053069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Numbers whose Italian name starts with "d".
   The sequence consists of the integer intervals
   [2·1000^s , 3·1000^s – 1],
   [10·1000^s , 11·1000^s – 1],
   [12·1000^s , 13·1000^s – 1],
   [17·1000^s , 18·1000^s – 1],
   [18·1000^s , 19·1000^s – 1],
   [19·1000^s , 20·1000^s – 1],
   [200·1000^s , 300·1000^s – 1]  for s = 0,1,2,...
   This function returns the n-th element (offset 0). */

seq_t a(idx_t n)
{
    seq_t pow = 1;                /* 1000^s  */
    idx_t idx = n;                 /* remaining index   */

    for (idx_t s = 0; ; ++s)
    {
        /* sizes of the six singleton ranges for this scale */
        idx_t single_size = (idx_t)pow;            /* pow fits into idx_t for n up to 10 000 */
        /* size of the 200–299 range for this scale */
        idx_t gsize       = 100 * single_size;      /* 100*pow */

        idx_t block_total = 6 * single_size + gsize;   /* total in this scale */
        if (idx >= block_total) {
            idx -= block_total;
            pow *= 1000;          /* advance to next scale */
            continue;
        }

        /* the desired element lies in the current scale */

        if (idx < single_size)      return ((seq_t)2) * pow + ((seq_t)idx);
        idx -= single_size;
        if (idx < single_size)      return ((seq_t)10) * pow + ((seq_t)idx);
        idx -= single_size;
        if (idx < single_size)      return ((seq_t)12) * pow + ((seq_t)idx);
        idx -= single_size;
        if (idx < single_size)      return ((seq_t)17) * pow + ((seq_t)idx);
        idx -= single_size;
        if (idx < single_size)      return ((seq_t)18) * pow + ((seq_t)idx);
        idx -= single_size;
        if (idx < single_size)      return ((seq_t)19) * pow + ((seq_t)idx);
        idx -= single_size;
        /* remaining indices belong to the 200–299 block */
        return ((seq_t)200) * pow + ((seq_t)idx);
    }
}
