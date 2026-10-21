/*
 * A069964  a(n) = (sum of digits of n)^4 - (sum of digits^4 of n).
 * https://oeis.org/A069964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (sum of digits of n)^4 – sum of (digit^4) for all digits
 *            of n
 * Range:     u8 n <= 13, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A069964 */
seq_t a(idx_t n)
{
    /* Sum of digits and sum of fourth powers of digits */
    seq_t sum     = 0;
    seq_t sum_four = 0;
    idx_t x = n;

    /* Handle n = 0 explicitly */
    if (x == 0)
    {
        /* sum = 0; sum_four = 0; */
        return 0;
    }

    /* Process each decimal digit of n */
    while (x != 0U)
    {
        /* Extract last decimal digit */
        int d = (int)(x % 10ULL);
        sum += d;
        /* Compute d^4 = d*d*d*d safely in seq_t */
        seq_t d_seq = (seq_t)d;
        seq_t d2 = d_seq * d_seq;        /* d^2 */
        seq_t d4 = d2 * d2;              /* d^4 */
        sum_four += d4;
        x /= 10ULL;
    }

    /* Compute (sum)^4 */
    seq_t sum_square = sum * sum;          /* sum^2 */
    seq_t sum_fourth = sum_square * sum_square;   /* (sum^2)^2 = sum^4 */

    /* Result = sum^4 - sum_of_digit_fourths */
    return sum_fourth - sum_four;
}
