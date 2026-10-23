/*
 * A039112  Numbers whose base-10 representation has the same number of 0's
 *          and 1's.
 * https://oeis.org/A039112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039112: numbers whose decimal representation has the same number of 0's and 1's
   Compute by a brute‑force search. */
seq_t a(idx_t n)
{
    idx_t found = 0;        /* number of terms found so far */
    seq_t value = 1;        /* current number being tested */

    while (found < n) {
        ++value;            /* test next integer */

        /* count 0's and 1's in decimal representation */
        seq_t x = value;
        unsigned char zeros = 0;
        unsigned char ones  = 0;
        while (x > 0) {
            uint8_t d = (uint8_t)(x % 10);
            if (d == 0) ++zeros;
            else if (d == 1) ++ones;
            x /= 10;
        }

        if (zeros == ones) {
            ++found;
            if (found == n)
                return value;
        }
    }
}
