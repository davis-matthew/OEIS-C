/*
 * A043393  Numbers whose base-7 representation contains exactly one 0.
 * https://oeis.org/A043393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑7 representation
 *            contains exactly one digit 0
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t found = 0;          /* how many suitable numbers we have seen   */
    seq_t v = 0;              /* the current integer being tested         */

    for (;;) {
        v++;                  /* test next integer                        */

        seq_t t = v;
        int zeros = 0;        /* count zeros in base‑7 representation     */
        while (t > 0) {
            if ((t % 7) == 0) {
                ++zeros;
                if (zeros > 1) break;   /* stop early if more than one zero */
            }
            t /= 7;
        }

        if (zeros == 1) {
            ++found;
            if (found == n) return v;  /* found the requested term */
        }
    }
}
