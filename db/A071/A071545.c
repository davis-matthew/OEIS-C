/*
 * A071545  Smallest k such that n+k divides (k+1)!+k!.
 * https://oeis.org/A071545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest positive k such that n+k | ((k+1)! + k!) = k! *
 *            (k+2)
 * Range:     u8 n <= 239, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* iterate k from 1 upwards until the divisibility holds */
    for (idx_t k = 1; ; k++)
    {
        /* d = n + k is the divisor we test */
        idx_t d = n + k;
        seq_t D = (seq_t)d;               /* promote to seq_t for arithmetic */

        /* compute k! mod d incrementally */
        seq_t fact = 1 % D;
        for (seq_t i = 1; i <= (seq_t)k; i++)
            fact = (fact * i) % D;

        /* compute (k! * (k+2)) mod d */
        seq_t val = (fact * ((seq_t)((k + 2) % d))) % D;

        if (val == 0)
            return (seq_t)k;
    }
}
