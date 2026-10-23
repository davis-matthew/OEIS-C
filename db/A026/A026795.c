/*
 * A026795  Expansion of 1/((1-2*x)*(1-6*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A026795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (875*10^n - 972*9^n + 126*6^n - 2^n)/28
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values a(0)..a(3) */
    seq_t a0 = 1, a1 = 27, a2 = 475, a3 = 6915;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t tmp = 27 * a3;       /* a(n-1) * 27  */
        tmp -= 254 * a2;           /* -254 * a(n-2) */
        tmp += 948 * a1;           /* +948 * a(n-3) */
        tmp -= 1080 * a0;          /* -1080 * a(n-4) */

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = tmp;
    }

    return a3;
}
