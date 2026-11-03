/*
 * A008752  Expansion of (1+x^9)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + p(n-9) where p(n) = number of non‑negative integer
 *            solutions of a+2b+3c = n (i.e. partitions of n using parts
 *            1,2,3)
 * Range:     u8 n <= 40, u16 n <= 628, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 963 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_parts_1_2_3(idx_t n)
{
    seq_t total = 0;
    for (idx_t c = 0; c <= n / 3; ++c) {
        idx_t rem = n - 3 * c;          /* remaining after choosing c */
        seq_t add = (seq_t)(rem / 2) + 1;/* number of (a,b) solutions for this c */
        total += add;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t res = count_parts_1_2_3(n);
    if (n >= 9) {
        res += count_parts_1_2_3(n - 9);
    }
    return res;
}
