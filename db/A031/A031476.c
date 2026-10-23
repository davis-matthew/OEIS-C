/*
 * A031476  Numbers whose base-5 representation has 3 fewer 0's than 4's.
 * https://oeis.org/A031476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation has 3 more 4's than 0's
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Index n is 1‑based.  Scan upwards finding numbers whose base‑5
       representation satisfies (#4) – (#0) == 3.  The search terminates
       when the n‑th such number is found.  This is efficient because the
       density of such numbers is roughly 1/5, and for n up to 10 000 the
       value is < 4×10⁵, so the loop runs only a few hundred thousand
       iterations. */
    idx_t count = 0;
    seq_t x = 0;
    for (;;) {
        x = x + 1;          /* next candidate */
        seq_t t = x;
        int zeros = 0;
        int fours = 0;
        while (t > 0) {
            int d = (int)(t % 5);  /* digit (0..4) */
            if (d == 0) zeros++;
            else if (d == 4) fours++;
            t = t / 5;
        }
        if (fours - zeros == 3) {
            count++;
            if (count == n) return x;
        }
    }
}
