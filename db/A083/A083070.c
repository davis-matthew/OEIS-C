/*
 * A083070  First superdiagonal of number array A083064.
 * https://oeis.org/A083070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n-1)*(n+1)^n + 1)/n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* the sequence is defined for n >= 1; return 0 for n == 0 as a sentinel */
        return (seq_t)0;
    }

    seq_t sn = (seq_t)n;            /* cast index to seq_t for arithmetic */
    seq_t base = sn + (seq_t)1;     /* (n+1) */

    /* compute (n+1)^n using binary exponentiation */
    seq_t power = (seq_t)1;
    idx_t exp = n;
    while (exp != 0) {
        if (exp & 1U)
            power = power * base;
        base = base * base;
        exp >>= 1;
    }

    seq_t num = (sn - (seq_t)1) * power + (seq_t)1;
    return num / sn;                /* division is exact by the formula */
}
