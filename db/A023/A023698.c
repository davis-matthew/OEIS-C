/*
 * A023698  Numbers with exactly 7 1's in their ternary expansion.
 * https://oeis.org/A023698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers with exactly seven 1's in their ternary expansion (OEIS
 *            A023698). This implementation scans integers in increasing order
 *            and counts the 1's in each ternary representation until the n‑th
 *            occurrence is found
 * Range:     u8 none, u16 n <= 1072, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static unsigned count_ones(seq_t x)
{
    unsigned cnt = 0;
    seq_t t = x;
    const seq_t three = (seq_t)3;
    const seq_t one   = (seq_t)1;
    while (t) {
        if (t % three == one) ++cnt;
        t /= three;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, return 0 as sentinel */
    seq_t val = 1;
    idx_t  found = 0;
    for (;;) {
        if (count_ones(val) == 7) {
            ++found;
            if (found == n) return val;
        }
        ++val;
    }
}
