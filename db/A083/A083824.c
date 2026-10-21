/*
 * A083824  a(n) = digit reversal of 9*n, divided by 9.
 * https://oeis.org/A083824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = rev(9*n) / 9
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where rev(x) is the decimal digit reversal of x */
seq_t a(idx_t n)
{
    seq_t prod = (seq_t)n * (seq_t)9;   /* 9*n */
    seq_t rev  = 0;
    seq_t temp = prod;

    /* reverse the digits of prod */
    while (temp > 0) {
        seq_t digit = temp % (seq_t)10; /* last decimal digit */
        rev = rev * (seq_t)10 + digit;  /* append digit */
        temp = temp / (seq_t)10;        /* drop last digit */
    }

    /* divide by 9 (exact division) */
    return rev / (seq_t)9;
}
