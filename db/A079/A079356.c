/*
 * A079356  a(1)=1; a(n) = a(n-1) - 2 if n is already in the sequence, a(n) =
 *          a(n-1) + 3 otherwise.
 * https://oeis.org/A079356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-3 + b(n) where b(n) is the period‑10 sequence (3, 5, 7,
 *            4, 1, 3, 0, 2, 4, 1)
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the sequence definition */
    static const seq_t period[10] = {
        (seq_t)3, (seq_t)5, (seq_t)7, (seq_t)4, (seq_t)1,
        (seq_t)3, (seq_t)0, (seq_t)2, (seq_t)4, (seq_t)1
    };

    idx_t idx = (n - 1) % 10;        /* index into the period array */
    seq_t res = period[idx];        /* start with the periodic part   */
    res += (seq_t)n;                /* add n                           */
    res -= 3;                       /* subtract the constant 3         */
    return res;                     /* a(n) = (n‑3) + b(n)             */
}
