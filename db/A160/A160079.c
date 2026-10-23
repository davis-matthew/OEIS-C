/*
 * A160079  Lodumo_3 of Fibonacci numbers.
 * https://oeis.org/A160079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 6*(n/8) if n mod 8 = 0 { 9*(n/8)+1 if n mod 8 = 1 {
 *            9*(n/8)+4 if n mod 8 = 2 { 9*(n/8)+2 if n mod 8 = 3 { 6*(n/8)+3
 *            if n mod 8 = 4 { 9*(n/8)+5 if n mod 8 = 5 { 9*(n/8)+8 if n mod 8
 *            = 6 { 9*(n/8)+7 if n mod 8 = 7
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n >> 3;       /* n / 8 */
    idx_t r = n & 7;        /* n % 8 */
    seq_t K = (seq_t)k;
    switch (r) {
        case 0: return 6 * K;
        case 1: return 9 * K + 1;
        case 2: return 9 * K + 4;
        case 3: return 9 * K + 2;
        case 4: return 6 * K + 3;
        case 5: return 9 * K + 5;
        case 6: return 9 * K + 8;
        case 7: return 9 * K + 7;
        default: return 0;   /* unreachable */
    }
}
