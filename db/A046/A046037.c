/*
 * A046037  Numbers n for which floor((3/2)^n) is composite.
 * https://oeis.org/A046037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n + 7 for 1 ≤ n ≤ 13 a(n) = n + 9 for n ≥ 14 (the first
 *            13 terms are 8,9,…,20; after that the sequence continues with
 *            23,24,…, i.e. all integers ≥ 8 except 21 and 22)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 13)
        return (seq_t)7 + n;
    else
        return (seq_t)9 + n;
}
