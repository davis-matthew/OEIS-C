/*
 * A076602  a(1)=7, a(n_even)=(a(n-1)^2-1)/2; a(n_odd)=a(n-1)+1.
 * https://oeis.org/A076602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=7; a(n)=a(n-1)+1 if n is odd; a(n)=(a(n-1)^2-1)/2 if n is
 *            even
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)7;          /* a(1) */
    if (n <= 1) return val;        /* already first term */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        if (i & (idx_t)1) {        /* odd n  →  a(n)=a(n-1)+1 */
            val = val + (seq_t)1;
        } else {                  /* even n →  a(n)=(a(n-1)^2-1)/2 */
            seq_t tmp = val * val; /* square */
            tmp = tmp - (seq_t)1;
            val = tmp / (seq_t)2;
        }
    }
    return val;
}
