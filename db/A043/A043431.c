/*
 * A043431  Numbers having three 2's in base 8.
 * https://oeis.org/A043431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate integers to find n‑th positive integer whose
   base‑8 representation contains exactly three digits equal to 2. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */
    idx_t found = 0;
    for (seq_t val = 0; ; ++val) {
        int cnt = 0;
        for (seq_t t = val; t != 0; t /= 8) {
            if ((t % 8) == 2) ++cnt;
        }
        if (cnt == 3) {
            ++found;
            if (found == n) return val;
        }
    }
}
