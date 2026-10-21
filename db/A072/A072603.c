/*
 * A072603  Numbers which in base 2 have more 0's than 1's.
 * https://oeis.org/A072603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 5202, u32 n <= 5202, u64 n <= 5202,
 *            u128 n <= 5202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A072603: Numbers whose binary representation has more 0’s than 1’s.
    a(1) = 4, a(2) = 8, …  The following implementation finds the n‑th term
    by brute–force scanning of integers, counting ones and zeros in their
    binary forms.  This approach works for all supported underlying
    integer types (fixed‑size or arbitrary‑precision). */
seq_t a(idx_t n) {
    /* counter of terms found so far */
    idx_t found = 0;
    /* candidate integer being tested */
    seq_t val = 1;

    while (1) {
        seq_t t = val;
        idx_t ones = 0, bits = 0;

        /* count bits and ones in val */
        if (val == 0) {
            bits = 1;                /* only the single zero bit */
        } else {
            while (t) {
                ones   += (idx_t)(t & 1);  /* add least significant bit */
                t >>= 1;
                bits++;
            }
        }

        /* more zeros than ones?  (bits - ones) gives zero count */
        if (bits > ones && (bits - ones) > ones) {
            found++;
            if (found == n)
                return val;
        }

        /* move to next integer */
        val++;
    }
}
