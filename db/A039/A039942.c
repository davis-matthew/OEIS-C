/*
 * A039942  Number of points needed to display n on the LCD (liquid-crystal
 *          display) of a TI-83 calculator.
 * https://oeis.org/A039942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = Σ a(d) (d runs over the decimal digits of n), with
 *            a(0)=19, a(1)=10, a(2)=14, a(3)=14, a(4)=14, a(5)=17, a(6)=15,
 *            a(7)=11, a(8)=17, a(9)=15 i.e. a(10*n+u) = a(n) + a(u)
 * Range:     u8 n <= 9640, u16 n <= 9640, u32 n <= 9640, u64 n <= 9640,
 *            u128 n <= 9640, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t digit_val[10] = {
    19, /* a(0) */
    10, /* a(1) */
    14, /* a(2) */
    14, /* a(3) */
    14, /* a(4) */
    17, /* a(5) */
    15, /* a(6) */
    11, /* a(7) */
    17, /* a(8) */
    15  /* a(9) */
};

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 explicitly. */
    if (n == 0)
        return digit_val[0];

    seq_t sum = 0;
    while (n != 0) {
        /* Work with the least‑significant decimal digit. */
        idx_t d = n % 10;      /* 0 <= d <= 9 */
        sum += digit_val[(unsigned int)d];
        n   /= 10;
    }
    return sum;
}
