/*
 * A166748  E.g.f.: exp(6*arcsin(x)).
 * https://oeis.org/A166748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 6, a(n+2) = (n^2 + 36) * a(n) for n >= 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial initial values */
    if (n == 0)
        return 1;
    if (n == 1)
        return 6;

    /* For even indices use a(0)=1 as the seed */
    if ((n & 1) == 0) {        /* n is even */
        seq_t val = 1;         /* a(0) */
        for (idx_t k = 2; k <= n; k += 2) {
            seq_t mult = (seq_t)(k - 2) * (seq_t)(k - 2) + 36;
            val = val * mult;  /* a(k) = mult * a(k-2) */
        }
        return val;            /* a(n) for even n */
    }
    /* For odd indices use a(1)=6 as the seed */
    else {                    /* n is odd */
        seq_t val = 6;         /* a(1) */
        for (idx_t k = 3; k <= n; k += 2) {
            seq_t mult = (seq_t)(k - 2) * (seq_t)(k - 2) + 36;
            val = val * mult;  /* a(k) = mult * a(k-2) */
        }
        return val;            /* a(n) for odd n */
    }
}
