/*
 * A105115  Numbers k such that the decimal representation of 1/k is neither
 *          terminating nor purely repeating.
 * https://oeis.org/A105115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105115: numbers k such that 1/k has a decimal expansion that is neither
   terminating nor purely repeating.  k must be divisible by 2 or 5 and have at
   least one additional prime factor (i.e., after removing all factors 2 and 5,
   a remainder > 1 remains).  Computed by scanning integers in increasing
   order and selecting those that satisfy the stated conditions. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety for out‑of‑range input */

    idx_t count = 0;                     /* how many valid k we have found */
    seq_t k = 0;                         /* current integer being tested */

    /* Start from 1 and increment k until we have found n valid terms. */
    for (k = 1;; ++k)
    {
        /* k must be divisible by 2 or by 5. */
        if ((k % 2 == 0) || (k % 5 == 0))
        {
            seq_t tmp = k;
            /* Remove all factors of 2. */
            while ((tmp % 2) == 0)
                tmp /= 2;
            /* Remove all factors of 5. */
            while ((tmp % 5) == 0)
                tmp /= 5;

            /* After removing all 2's and 5's we must have a remaining factor
               other than 1.  That guarantees that 1/k is neither terminating
               nor purely repeating. */
            if (tmp > (seq_t)1)
            {
                ++count;
                if (count == n)
                    return k;
            }
        }
    }
}
