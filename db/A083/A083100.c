/*
 * A083100  a(n) = 2*a(n-1) + 7*a(n-2).
 * https://oeis.org/A083100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+7*a(n-2), with a(0)=1, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* companion matrix for the recurrence:
       | 2 7 |
       | 1 0 | */
    typedef struct {
        seq_t a, b, c, d;  /* [ [a, b], [c, d] ] */
    } Mat;

    const Mat M = { (seq_t)2, (seq_t)7, (seq_t)1, (seq_t)0 };
    Mat res = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };  /* identity */
    Mat base = M;
    idx_t exp = n - 1;

    while (exp) {
        if (exp & 1) {
            Mat tmp;
            tmp.a = res.a * base.a + res.b * base.c;
            tmp.b = res.a * base.b + res.b * base.d;
            tmp.c = res.c * base.a + res.d * base.c;
            tmp.d = res.c * base.b + res.d * base.d;
            res = tmp;
        }
        {
            Mat tmp;
            tmp.a = base.a * base.a + base.b * base.c;
            tmp.b = base.a * base.b + base.b * base.d;
            tmp.c = base.c * base.a + base.d * base.c;
            tmp.d = base.c * base.b + base.d * base.d;
            base = tmp;
        }
        exp >>= 1;
    }

    /* a(n) = [res.a  res.b] * [a(1); a(0)] = res.a*9 + res.b*1 */
    return res.a * (seq_t)9 + res.b * (seq_t)1;
}
