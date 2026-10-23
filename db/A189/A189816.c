/*
 * A189816  a(3*k-2)=0, a(3*k-1)=1-a(k), a(3*k)=1-a(k); k>0, a(1)=0.
 * https://oeis.org/A189816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(3*k-2)=0, a(3*k-1)=1-a(k), a(3*k)=1-a(k); a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    switch (n % 3) {
        case 1: /* a(3*k-2) = 0 */
            return 0;
        case 2: /* a(3*k-1) = 1 - a(k) */
            return (seq_t)1 - a((n + 1) / 3);
        case 0: /* a(3*k)   = 1 - a(k) */
            return (seq_t)1 - a(n / 3);
    }
    return 0; /* unreachable, but required for strict C compliance */
}
