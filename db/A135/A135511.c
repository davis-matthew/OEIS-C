/*
 * A135511  Number of Pierce-Engel hybrid expansions of 3/b, b>=3.
 * https://oeis.org/A135511
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = [1,3,2,1,2,3] for n mod 6 (offset 3)
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    idx_t r = n % (idx_t)6;
    switch (r) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)3;
        case 2: return (seq_t)2;
        case 3: return (seq_t)1;
        case 4: return (seq_t)2;
        case 5: return (seq_t)3;
    }
    return 0; /* unreachable */
}
