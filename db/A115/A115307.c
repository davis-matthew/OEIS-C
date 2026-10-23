/*
 * A115307  a(n) = if n<7 then n else 7*a(floor(n/7)) + 6 - n mod 7.
 * https://oeis.org/A115307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for n<7; a(n) = 7*a(floor(n/7)) + 6 - n mod 7 for n>=7;
 *            initial values: a(1)=1,a(2)=2,a(3)=3,a(4)=4,a(5)=5,a(6)=6
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7)
        return (seq_t)n;

    idx_t tmp = n;
    idx_t digits[32];
    int cnt = 0;
    while (tmp > 0) {
        digits[cnt++] = tmp % 7;
        tmp /= 7;
    }

    seq_t res = (seq_t)digits[cnt - 1];
    for (int i = cnt - 2; i >= 0; --i) {
        res = res * 7 + ((seq_t)6 - (seq_t)digits[i]);
    }
    return res;
}
