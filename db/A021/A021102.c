/*
 * A021102  Decimal expansion of 1/98.
 * https://oeis.org/A021102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, for n>=1 a(n)=⌊10^(n+1)/98⌋−10⌊10^n/98⌋
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the n-th term of OEIS A021102 – the decimal digits of 1/98,
   with a(0) equal to the integer part (0). */
seq_t a(idx_t n)
{
    /* The integer part (term 0) */
    if (n == 0)
        return 0;

    /* Generate the (n+1)-th decimal digit of 1/98.
       The digit is the (n+1)-th digit after the decimal point,
       because the first decimal digit (0) is omitted from the
       sequence. */
    idx_t rem = 1;            /* current remainder (initially 1 for 1/98) */
    seq_t digit = 0;          /* will hold the digit to return */
    const idx_t steps = n + 1;/* number of long–division steps needed */

    for (idx_t i = 0; i < steps; ++i) {
        rem *= 10;            /* bring down the next decimal place */
        digit = rem / 98;     /* integer part of the division = the digit */
        rem %= 98;            /* new remainder for the next step */
    }

    return digit;
}
