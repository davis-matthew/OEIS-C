/*
 * A217402  Numbers starting with 9.
 * https://oeis.org/A217402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (80*10^floor(log10(9*n-8)) - 8)/9
 * Range:     u8 n <= 11, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t m  = tn * (seq_t)9 - (seq_t)8;
    seq_t tmp = m;
    idx_t d = 0;
    while (tmp >= (seq_t)10) {
        tmp /= (seq_t)10;
        d++;
    }
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < d; ++i) {
        pow10 *= (seq_t)10;
    }
    seq_t term = ((seq_t)80 * pow10 - (seq_t)8) / (seq_t)9;
    return tn + term;
}
