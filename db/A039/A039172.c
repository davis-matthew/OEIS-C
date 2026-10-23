/*
 * A039172  Numbers whose base-11 representation has the same number of 1's
 *          and 9's.
 * https://oeis.org/A039172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute force: iterate integers and count 1's and 9's in base-11 representation. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 0;
    for (;; ++x) {
        seq_t temp = (seq_t)x;
        int ones = 0;
        int nines = 0;
        while (temp) {
            seq_t d = temp % (seq_t)11;
            if (d == 1) ++ones;
            else if (d == 9) ++nines;
            temp /= (seq_t)11;
        }
        if (ones == nines) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
