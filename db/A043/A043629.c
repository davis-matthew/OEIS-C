/*
 * A043629  Numbers whose base-8 representation has exactly 7 runs.
 * https://oeis.org/A043629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A043629 – numbers whose base‑8 representation has exactly 7 runs.
   Computation via a straightforward linear search. */
seq_t a(idx_t n) {
    const seq_t FIRST = 266305;          /* 1010101₈ */
    idx_t count = 0;
    seq_t num = FIRST;
    while (1) {
        /* Count runs in base‑8 representation of num */
        seq_t runs = 0;
        int prev = -1;
        seq_t temp = num;
        while (temp > 0) {
            int d = (int)(temp % 8);
            if (d != prev) {
                runs++;
                prev = d;
            }
            temp /= 8;
        }
        if (runs == 7) {
            count++;
            if (count == n)
                return num;
        }
        num++;
    }
}
