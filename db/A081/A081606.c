/*
 * A081606  Numbers having at least one 1 in their ternary representation.
 * https://oeis.org/A081606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑3 representation
 *            contains at least one digit 1 The algorithm scans numbers
 *            sequentially and counts those that have a digit 1 in base 3
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; n==0 can be treated as 0 */
    idx_t counted = 0;
    seq_t m = 0;
    for (;;) {
        ++m;
        seq_t t = m;
        int has_one = 0;
        while (t) {
            if (t % 3 == 1) { has_one = 1; break; }
            t /= 3;
        }
        if (has_one) {
            ++counted;
            if (counted == n) return m;
        }
    }
}
