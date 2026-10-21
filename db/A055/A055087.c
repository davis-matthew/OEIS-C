/*
 * A055087  Integers 0..n then 0..n then 0..n+1 then 0..n+1 etc.
 * https://oeis.org/A055087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   Let k = ⌊(√(8·n+1)−1)/2⌋. Then a(n) = (n−k(k+1)) mod (k+1). The
 *            sequence is produced by repeating each row [0,1,…,k] twice in
 *            order
 * Range:     u8 n <= 3720, u16 n <= 3720, u32 n <= 3720, u64 n <= 3720,
 *            u128 n <= 3720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find k such that k*(k+1) ≤ n < (k+1)*(k+2).  Binary search on k. */
    idx_t low  = 0;
    idx_t high = n + 1;                     /* upper bound is safe */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        if (prod <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t k = low - 1;                       /* k satisfies k*(k+1) ≤ n */

    idx_t start   = k * (k + 1);             /* first index of this block pair */
    idx_t offset  = n - start;               /* 0 ≤ offset < 2*(k+1)          */
    idx_t period  = k + 1;                   /* length of one copy of the row */
    idx_t value   = offset % period;         /* 0 ≤ value ≤ k                 */

    return (seq_t)value;
}
