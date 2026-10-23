/*
 * A043847  Number of runs in the base 4 representation of n is congruent to 4
 *          mod 7.
 * https://oeis.org/A043847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer m such that the number of runs in the base‑4
 *            representation of m is congruent to 4 (mod 7)
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t count = 0;          /* how many qualifying numbers found */
    seq_t cur = 1;            /* current integer to test */

    while (1) {
        /* count runs modulo 7 in the base‑4 representation of cur */
        seq_t x = cur;
        int runs_mod7 = 0;    /* runs count modulo 7 */
        int prev_digit = -1;  /* no previous digit yet */

        while (x > 0) {
            int digit = (int)(x % 4);   /* 0…3 */
            if (digit != prev_digit) {
                runs_mod7 = (runs_mod7 + 1) % 7;
                prev_digit = digit;
            }
            x = x / 4;
        }

        if (runs_mod7 == 4) {     /* cur satisfies the property */
            count++;
            if (count == n) {
                return cur;
            }
        }

        cur++;                    /* test next integer */
    }
}
