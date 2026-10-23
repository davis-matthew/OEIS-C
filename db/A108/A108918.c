/*
 * A108918  Reversed binary words in reversed lexicographic order.
 * https://oeis.org/A108918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2^(m+1)-1) = 2^m; a(2^m + k) = a(k+1) + 2^m for 0 <= k < 2^m-1
 *            (n >= 1)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle only n >= 1; for n == 0 return 0 (undefined in OEIS) */
    if (n == 0)
        return (seq_t)0;

    /* Determine the largest power of two <= n */
    idx_t pow = 1;
    while (pow <= n / 2)
        pow <<= 1;                /* pow = 2^m */

    /* Special case: n == 2^(m+1)-1  <=>  pow == (n+1)/2      */
    if (pow == (n + 1) / 2)
        return (seq_t)pow;        /* a(2^(m+1)-1) = 2^m */

    /* General case: n = 2^m + k   (0 <= k < 2^m-1)            */
    idx_t k = n - pow;            /* k = n - 2^m   */
    return ((seq_t)a(k + 1) + (seq_t)pow);
}
