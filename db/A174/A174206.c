/*
 * A174206  Write natural numbers in base 2 as a stream of digits. Moving left
 *          to right, delete even occurrences of digit 0 and 1.
 * https://oeis.org/A174206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, uned
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A174206
   This function simulates the binary stream formed by writing the natural
   numbers 0,1,2,… in base‑2, then scans the stream left to right and
   deletes every even occurrence of each digit separately.
   It returns the n‑th surviving digit (0 or 1); a(0) = 0, a(1) = 1, … */
seq_t a(idx_t n)
{
    /* Counters for the number of times 0 and 1 have been seen so far. */
    idx_t occ[2] = {0,0};

    /* Index of the survivor we are looking for. */
    idx_t survivor_idx = 0;

    /* Current natural number to write in binary. */
    idx_t k = 0;

    for (;;) {
        if (k == 0) {                     /* special case: "0" */
            occ[0]++;                     /* first occurrence of 0 */
            if (occ[0] & 1) {             /* keep it if odd count */
                if (survivor_idx == n) return (seq_t)0;
                survivor_idx++;
            }
        } else {
            /* Find most significant bit position of k. */
            idx_t t = k;
            int msb = 0;
            while (t >>= 1) msb++;

            /* Process bits from msb down to 0. */
            for (int pos = msb; pos >= 0; --pos) {
                unsigned int bit = (k >> pos) & 1U; /* 0 or 1 */
                occ[bit]++;
                if (occ[bit] & 1) {             /* odd count -> keep */
                    if (survivor_idx == n) return (seq_t)bit;
                    survivor_idx++;
                }
            }
        }
        ++k;
    }
}
