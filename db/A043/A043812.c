/*
 * A043812  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 6 mod 9.
 * https://oeis.org/A043812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑3 representation has a
 *            number of runs congruent to 6 modulo 9
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0, harmless if called */

    idx_t found = 0;          /* how many numbers have satisfied the condition so far */
    seq_t x = 1;              /* candidate integer to test */

    for (;; ++x) {
        seq_t y   = x;
        int   runs = 0;
        int   prev_digit = -1;   /* -1 means no previous digit */

        while (y > 0) {
            int d = (int)(y % (seq_t)3);
            if (d != prev_digit) {
                runs++;
                prev_digit = d;
            }
            y /= (seq_t)3;
        }

        if (runs % 9 == 6) {
            ++found;
            if (found == n) return x;
        }
    }
}
