/*
 * A097403  Minimum wind speed in knots for Beaufort Number n.
 * https://oeis.org/A097403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 0 (n=0); 1 (n=1); 4 (n=2); 7 (n=3); 11 (n=4); for n>=5
 *            let m=n-5, p=floor(m/2); if m is even a=p*p+10p+17 else
 *            a=p*p+11p+22
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        switch ((idx_t)n) {
            case 0: return 0;
            case 1: return 1;
            case 2: return 4;
            case 3: return 7;
            case 4: return 11;
            default: return 0; /* unreachable */
        }
    }
    seq_t m = (seq_t)(n - 5);      /* m = n-5 */
    seq_t p = m / 2;               /* p = floor(m/2) */
    if (m % 2 == 0)                /* m even */
        return p * p + 10 * p + 17;
    else                            /* m odd */
        return p * p + 11 * p + 22;
}
