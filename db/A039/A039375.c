/*
 * A039375  Numbers whose base-9 representation has the same nonzero number of
 *          2's and 6's.
 * https://oeis.org/A039375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this sequence is defined by searching for integers whose *
 *            base‑9 representation contains the same non‑zero number of
 *            digits 2 * and 6. This implementation scans successive integers,
 *            counting * qualifying ones until the nth is reached. It works
 *            for any * seq_t/idx_t configuration supported by the...
 * Range:     u8 n <= 18, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>               /* for uint8_t */

seq_t a(idx_t n)
{
    if (n == 0)                /* the offset is 1, but a(0) is unused */
        return (seq_t)0;

    idx_t count = 0;           /* how many valid numbers found so far */
    idx_t cand   = 0;           /* current integer being tested */

    while (count < n) {
        ++cand;                /* examine next candidate */

        /* Count the 2's and 6's in the base‑9 representation of cand. */
        idx_t tmp  = cand;
        int c2 = 0, c6 = 0;
        while (tmp > 0) {
            uint8_t d = tmp % 9;
            if (d == 2)
                ++c2;
            else if (d == 6)
                ++c6;
            tmp /= 9;
        }

        /* Accept if the numbers of 2's and 6's are equal and non‑zero. */
        if (c2 == c6 && c2 > 0)
            ++count;
    }

    return (seq_t)cand;        /* cast back to the generic integer type */
}
