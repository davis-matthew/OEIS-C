/*
 * A144789  Consider the runs of 0's in the binary representation of n, each
 *          of these runs being on the edge of the binary representation n
 *          and/or being bounded by 1's. a(n) = the length of the shortest
 *          such run (with positive length) of 0's in binary n. a(n) = 0 if
 *          there are no runs of 0's in binary n.
 * https://oeis.org/A144789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = length of the shortest run of 0's in the binary
 *            representation of n (0 if n has no zero runs). a(1) = 0, a(2) =
 *            1, a(3) = 0, a(4) = 2,
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work on a copy of n so we can shift it without affecting the caller. */
    idx_t v = n;
    seq_t best = 0;            /* best (shortest) run found so far, 0 means none yet */
    idx_t run_len = 0;         /* length of the current run of zeros */
    int in_zero = 0;           /* flag: are we inside a run of zeros? */

    while (v) {
        int bit = v & 1;          /* take least‑significant bit */
        if (bit == 0) {
            run_len++;
            in_zero = 1;
        } else {                  /* bit == 1 */
            if (in_zero) {
                if (best == 0 || (seq_t)run_len < best)
                    best = run_len;     /* update shortest run */
                run_len = 0;
                in_zero = 0;
            }
        }
        v >>= 1;                  /* move to next higher bit */
    }

    /* No need to check in_zero after the loop because the most significant bit is 1,  
       so any trailing zeros have already been ended by a following 1. */

    return best;
}
