/*
 * A137529  a(n) = 8*a(n-4).
 * https://oeis.org/A137529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-4); a(0)=1, a(1)=3, a(2)=4, a(3)=4
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 84, u128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A137529, valid for all n >= 0. */
seq_t a(idx_t n) {
    /* Base terms for n mod 4. */
    static const seq_t init[4] = { (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)4 };

    /* Start from the base term and multiply by 8^(floor(n/4)). */
    seq_t res = init[n & 3];           /* n % 4 using bitmask */
    idx_t   times = n >> 2;            /* n / 4 for unsigned idx_t */

    while (times--) {
        res *= (seq_t)8;
    }

    return res;
}
