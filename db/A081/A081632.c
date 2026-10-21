/*
 * A081632  a(n) = 2*3^n-(-1)^n.
 * https://oeis.org/A081632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n-(-1)^n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(0) = 1, a(1) = 7, ... */
seq_t a(idx_t n)
{
    /* Compute 3^n */
    seq_t pow3 = 1;
    seq_t base = 3;
    idx_t e = n;
    while (e) {
        if (e & 1) pow3 *= base;
        base *= base;
        e >>= 1;
    }

    /* 2*3^n */
    seq_t two_times = pow3 + pow3;

    /* Subtract or add 1 depending on parity of n */
    if ((n & 1) == 0) /* n even: (-1)^n = +1 */
        return two_times - 1;
    else              /* n odd:  (-1)^n = -1 */
        return two_times + 1;
}
