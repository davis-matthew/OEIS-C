/*
 * A021862  Decimal expansion of 1/858.
 * https://oeis.org/A021862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/858) mod 10
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (the n-th digit of the decimal expansion of 1/858) */
seq_t a(idx_t n)
{
    seq_t r   = 1;      /* current remainder in the long division 1/858 */
    seq_t dig = 0;      /* digit at the current position */

    for (idx_t i = 0; i <= n; ++i) {
        dig = (r * 10) / 858;     /* next decimal digit */
        r   = (r * 10) % 858;     /* update remainder for next step */
    }

    return dig;
}
