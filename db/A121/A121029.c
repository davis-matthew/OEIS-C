/*
 * A121029  Multiples of 9 containing a 9 in their decimal representation.
 * https://oeis.org/A121029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121029: multiples of 9 that contain the digit 9.
   Computed by brute‑force search of multiples of 9. */

seq_t a(idx_t n)
{
    idx_t cur = 1;          /* current multiple index */
    seq_t result = 0;
    idx_t count = 0;        /* how many qualifying numbers seen so far */

    for (;;) {
        result = ((seq_t)cur) * 9;   /* next multiple of 9 */

        seq_t tmp = result;
        int has9 = 0;
        while (tmp > 0) {
            if (tmp % 10 == 9) {     /* digit 9 found */
                has9 = 1;
                break;
            }
            tmp /= 10;
        }

        if (has9) {
            ++count;
            if (count == n)
                return result;
        }
        ++cur;
    }
}
