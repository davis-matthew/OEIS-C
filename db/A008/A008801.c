/*
 * A008801  Molien series for group [2,8]+ = 228.
 * https://oeis.org/A008801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n) + (n>=9 ? b(n-9) : 0) * where b(n) = 0 if n is odd,
 *            * and for even n: b(n) = (⌊n/8⌋+1) * (n/2 + 1 − 2⌊n/8⌋)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _b(idx_t n)
{
    if (n & 1)                /* odd n → no contributions */
        return 0;
    idx_t t = n / 8;          /* ⌊n/8⌋ */
    seq_t factor = (seq_t)(t + 1);
    seq_t inner  = (seq_t)(n / 2 + 1 - 2 * t);
    return factor * inner;
}
 
seq_t a(idx_t n)
{
    seq_t res = _b(n);
    if (n >= 9)
        res += _b(n - 9);
    return res;
}
