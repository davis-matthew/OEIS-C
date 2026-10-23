/*
 * A033284  Numbers whose maximal base-10 run length is 3.
 * https://oeis.org/A033284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth member of A033284 (Numbers whose maximal base‑10 run
   length is 3) by brute‑force enumeration of integers. */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1, but guard against zero */
    idx_t count = 0;
    seq_t num = 111;               /* first number with run length 3 */

    for (;;) {
        /* Determine the maximal run length of the decimal digits of num */
        seq_t x = num;
        seq_t maxr = 1;
        seq_t run = 1;
        seq_t prev = x % (seq_t)10;
        x /= (seq_t)10;

        while (x > 0) {
            seq_t d = x % (seq_t)10;
            if (d == prev) {
                run++;
            } else {
                run = 1;
                prev = d;
            }
            if (run > maxr) maxr = run;
            x /= (seq_t)10;
        }

        if (maxr == (seq_t)3) {
            count++;
            if (count == n) return num;
        }
        num++;                     /* next integer to test */
    }
}
