/*
 * A139378  Number of 2-digit multiples of n whose digits sum to n.
 * https://oeis.org/A139378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, full, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of 2‑digit multiples of n whose digits sum to n. */
seq_t a(idx_t n)
{
    seq_t cnt = 0;
    if (n == 0) return cnt;          /* n should be >= 1; guard against 0 */

    /* First multiple of n that is at least 10. */
    idx_t start = (10 + n - 1) / n;
    for (idx_t k = start; k * n <= 99; ++k)
    {
        idx_t val = k * n;                /* the 2‑digit multiple */
        idx_t s = 0;
        idx_t tmp = val;
        while (tmp > 0) {
            s += tmp % 10;
            tmp /= 10;
        }
        if (s == n) ++cnt;               /* sum of digits matches n */
    }
    return cnt;
}
