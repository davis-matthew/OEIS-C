/*
 * A047314  Numbers that are congruent to {0, 1, 4, 5, 6} mod 7.
 * https://oeis.org/A047314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A047314: Numbers that are congruent to {0,1,4,5,6} mod 7.
   Formula: a(n) = 7 * floor((n-1)/5) + residue[(n-1)%5]
   where residue = {0,1,4,5,6} */
   seq_t a(idx_t n)
   {
       /* the allowed residues in increasing order */
       static const unsigned residues[5] = {0U, 1U, 4U, 5U, 6U};

       /* guard against n==0, though by definition n>=1 is required */
       if (n == 0) {
           return (seq_t)0;
       }

       idx_t q = (n - 1) / 5;   /* number of complete 5‑element blocks */
       idx_t r = (n - 1) % 5;   /* position inside the block */

       /* compute the term using seq_t arithmetic only */
       seq_t block_start = (seq_t)7 * (seq_t)q;
       seq_t result = block_start + (seq_t)residues[r];
       return result;
   }
