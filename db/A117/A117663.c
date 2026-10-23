/*
 * A117663  Heptagonal numbers for which the digital root is also a heptagonal
 *          number.
 * https://oeis.org/A117663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117663: Heptagonal numbers whose digital root is a heptagonal number.
   The sequence is defined as the ascending list of H_k = k*(5k-3)/2 with
   digital root 1 or 7, together with H_0 = 0. */

/*  Include stdbool for bool. */
#include <stdbool.h>

/*  Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /*  H_0 = 0 is the first term.  */
    if (n == 1)
        return 0;

    /*  We already counted H_0.  */
    idx_t count = 1;   /* number of qualifying terms found so far */
    idx_t k      = 1;  /* current index for the heptagonal number */

    while (true)
    {
        /*  Heptagonal number H_k = k*(5k-3)/2.  */
        seq_t kk = (seq_t)k;
        seq_t H  = kk * (kk * 5 - 3) / 2;

        /*  Digital root: for H>0, H % 9 gives 1–9 (0 indicates 9).  */
        seq_t mod9 = H % 9;
        if (mod9 == 1 || mod9 == 7)
        {
            ++count;
            if (count == n)
                return H;  /* found the desired term */
        }

        ++k;  /* move to next heptagonal number */
    }
}
