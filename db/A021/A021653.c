/*
 * A021653  Decimal expansion of 1/649.
 * https://oeis.org/A021653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) / 649) mod 10 – the n‑th decimal digit of 1/649
 *            (0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider type for the remainder so that operations don't overflow */
    idx_t r = 1;          /* initial remainder is 1 */
    idx_t digit = 0;      /* current digit */

    for (idx_t i = 0; i <= n; ++i) {
        r = r * 10;          /* multiply by 10 to shift decimal place */
        digit = r / 649;     /* integer division gives the next digit */
        r = r % 649;         /* keep only the remainder for the next step */
    }

    return (seq_t)digit;    /* cast to the required return type */
}
