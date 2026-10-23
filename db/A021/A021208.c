/*
 * A021208  Decimal expansion of 1/204.
 * https://oeis.org/A021208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/204
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The decimal expansion is 0.004901960784313725490196078...     */
/* The digit is obtained by successive division:             */
/*   r = 1;                (initial remainder)\n*/
/*   for i = 0 to n:\n*/
/*     r = r * 10;          (shift left one decimal place)\n*/
/*     d = r / 204;         (next digit)\n*/
/*     r = r % 204;         (new remainder)\n*/
/*   return d;             (digit at position n)\n*/
seq_t a(idx_t n)
{
    seq_t remainder = 1;  /* initial remainder */
    seq_t digit = 0;      /* store the current digit */

    for (idx_t i = 0; i <= n; ++i)
    {
        remainder *= (seq_t)10;
        digit = remainder / (seq_t)204;
        remainder = remainder % (seq_t)204;
    }

    return digit;
}
