/*
 * A036191  Log base 6 (n) mod 151.
 * https://oeis.org/A036191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = discrete log base 6 of n modulo 151 (i.e. find k with 6^k
 *            ≡ n (mod 151), 0 ≤ k < 150)
 * Range:     u8 n <= 150, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static int init = 0;
    static seq_t logtable[151];

    if (!init) {
        seq_t v = 1;
        for (idx_t k = 0; k < 150; ++k) {
            logtable[(idx_t)v] = (seq_t)k;
            v = (v * 6) % 151;
        }
        init = 1;
    }

    idx_t r = n % 151;
    if (r == 0)
        return 0;  /* undefined for multiples of 151, return 0 as a sentinel */

    return logtable[(idx_t)r];
}
