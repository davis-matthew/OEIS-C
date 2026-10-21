/*
 * A064564  Reciprocal of n terminates with an infinite repetition of digit 5.
 *          Multiples of 10 are omitted.
 * https://oeis.org/A064564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers of the form 9*2^(6i‑5) and 9*5^(6i‑1) (i>0) merged in
 *            increasing order
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial elements of the two monotone lists */
    seq_t val2 = (seq_t)9 * (seq_t)2;          /* 9*2^(1)     = 18          */
    seq_t val5 = (seq_t)9 * (seq_t)3125;       /* 9*5^(5)     = 28125        */

    /* multipliers to advance each list */
    const seq_t factor2 = (seq_t)64;           /* 2^6          = 64          */
    const seq_t factor5 = (seq_t)15625;        /* 5^6          = 15625      */

    idx_t count = 0;
    seq_t next = 0;

    while (count < n) {
        if (val2 <= val5) {
            next = val2;
            val2 = val2 * factor2;  /* advance the 2‑power list      */
        } else {
            next = val5;
            val5 = val5 * factor5;  /* advance the 5‑power list      */
        }
        ++count;
    }
    return next;
}
