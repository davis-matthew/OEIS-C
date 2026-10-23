/*
 * A044942  Number of runs of odd length in the base-2 representation of n.
 * https://oeis.org/A044942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of runs of odd length in the binary representation of n.
   (Runs are sequences of equal bits; a run counts if its length is odd.) */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* not required but harmless */
    idx_t tmp = n >> 1;                  /* shift off least significant bit */
    int prev_bit = (int)(n & 1);         /* first bit */
    int len = 1;                         /* current run length */
    seq_t res = 0;                       /* result counter */

    while (tmp) {
        int bit = (int)(tmp & 1);
        if (bit == prev_bit) {
            len++;                       /* extend current run */
        } else {
            if ((len & 1) == 1)          /* odd length run finished */
                res++;
            prev_bit = bit;              /* start new run */
            len = 1;
        }
        tmp >>= 1;
    }

    if ((len & 1) == 1)                  /* count last run */
        res++;

    return res;
}
