/*
 * A080032  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is even".
 * https://oeis.org/A080032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=2, a(2)=4, a(3)=1; for n≥4: a(n) = 6*(n/4) if n≡0
 *            (mod 4); a(n) = 4*(n/4)+3 if n≡1 (mod 4); a(n) = 6*(n/4)+2 if
 *            n≡2 (mod 4); a(n) = 6*(n/4)+4 if n≡3 (mod 4)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)1;

    idx_t m = n / 4;
    seq_t res;
    switch (n % 4) {
        case 0:
            res = (seq_t)6 * (seq_t)m;
            break;
        case 1:
            res = (seq_t)4 * (seq_t)m + (seq_t)3;
            break;
        case 2:
            res = (seq_t)6 * (seq_t)m + (seq_t)2;
            break;
        case 3:
            res = (seq_t)6 * (seq_t)m + (seq_t)4;
            break;
        default:
            /* unreachable */
            res = (seq_t)0;
    }
    return res;
}
