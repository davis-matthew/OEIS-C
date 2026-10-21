/*
 * A233877  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          having the sum of the squares of all six edge and diagonal
 *          differences equal to 11.
 * https://oeis.org/A233877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 19*a(n-2) - 60*a(n-3) + 8*a(n-4) + 36*a(n-5) -
 *            12*a(n-6); Initial terms (n ≥ 1): a(1)=76, a(2)=300, a(3)=1224,
 *            a(4)=5156, a(5)=22020, a(6)=95464
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)76;
    if (n == 2) return (seq_t)300;
    if (n == 3) return (seq_t)1224;
    if (n == 4) return (seq_t)5156;
    if (n == 5) return (seq_t)22020;
    if (n == 6) return (seq_t)95464;

    seq_t a1 = (seq_t)76;
    seq_t a2 = (seq_t)300;
    seq_t a3 = (seq_t)1224;
    seq_t a4 = (seq_t)5156;
    seq_t a5 = (seq_t)22020;
    seq_t a6 = (seq_t)95464;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 3 * a6 + 19 * a5 - 60 * a4 + 8 * a3 + 36 * a2 - 12 * a1;
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6; a6 = next;
    }
    return a6;
}
