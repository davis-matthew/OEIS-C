/*
 * A021915  Decimal expansion of 1/911.
 * https://oeis.org/A021915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th decimal digit of 1/911 (offset 0) */
seq_t a(idx_t n)
{
    const seq_t denom = 911;
    seq_t r = 1;                     /* initial remainder before first digit */
    idx_t i;

    for (i = 0; i <= n; ++i) {
        r *= 10;                     /* bring next digit */
        seq_t d = r / denom;         /* digit to output */
        r = r % denom;               /* new remainder */

        if (i == n)
            return d;
    }
    /* should never reach here */
    return 0;
}
