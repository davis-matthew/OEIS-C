/*
 * A103627  Let S(n) = {n,1,n}; sequence gives concatenation S(0), S(1), S(2),
 *          ...
 * https://oeis.org/A103627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, if n mod 3 = 1; otherwise a(n) = n/3
 * Range:     u8 n <= 767, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9270 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 3ULL) == 1ULL)
        return (seq_t)1;
    return (seq_t)(n / 3ULL);
}
