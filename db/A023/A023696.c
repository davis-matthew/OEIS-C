/*
 * A023696  Numbers with exactly 5 1's in ternary expansion.
 * https://oeis.org/A023696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n­th integer whose base‑3 representation contains exactly five digits equal to 1.
   The routine simply scans integers in increasing order and counts 1‑digits in base‑3
   until the desired term is found. */

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t cand   = 0;   /* current candidate number */

    for (;;) {
        cand += 1;            /* next integer */

        seq_t tmp  = cand;
        idx_t  ones = 0;

        while (tmp > 0) {
            if ((tmp % 3) == 1)
                ++ones;
            tmp /= 3;
        }

        if (ones == 5) {
            ++found;
            if (found == n)
                return cand;
        }
    }
}
