/*
 * A139384  The number of 3-digit multiples of n whose digits sum to n.
 * https://oeis.org/A139384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, full, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the number of 3‑digit multiples of n whose digit sum equals n */
/* The function brute‑forces all multiples of n in the range [100, 999] */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1, but guard anyway */
        return 0;
    if (n > 999)                /* no 3‑digit multiple can be a multiple of n */
        return 0;

    seq_t count = 0;
    /* first multiple of n that is at least 100 */
    idx_t first = ((100 + n - 1) / n) * n;
    if (first > 999)
        return 0;

    for (idx_t m = first; m <= 999; m += n)
    {
        int sum = 0;
        idx_t tmp = m;
        while (tmp > 0)
        {
            sum += tmp % 10;
            tmp /= 10;
        }
        if (sum == (int)n)
            count++;
    }
    return count;
}
