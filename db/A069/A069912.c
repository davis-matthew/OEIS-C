/*
 * A069912  a(n) = A067552(n)/9 where A067552(n) = SumOfDigits(n)^2 -
 *          SumOfDigits(n^2), with SumOfDigits = A007953.
 * https://oeis.org/A069912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (SumDigits(n)^2 - SumDigits(n^2)) / 9
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SumDigits is the decimal digit sum (OEIS A007953).  */
seq_t a(idx_t n)
{
    /* compute SumDigits(n) */
    seq_t sum_n = 0;
    idx_t temp = n;
    while (temp)
    {
        sum_n += temp % 10;
        temp /= 10;
    }

    /* square of the digit sum */
    seq_t sum_n_sq = sum_n * sum_n;

    /* compute n^2 using 128‑bit arithmetic to avoid overflow
       (idx_t is 64‑bit, so n^2 fits in 128 bits). */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 sq = nn * nn;

    /* compute SumDigits(n^2) */
    seq_t sum_sq = 0;
    unsigned __int128 t = sq;
    while (t)
    {
        sum_sq += (unsigned)(t % 10);
        t /= 10;
    }

    /* result is always divisible by 9 */
    seq_t res = (sum_n_sq - sum_sq) / 9;
    return res;
}
