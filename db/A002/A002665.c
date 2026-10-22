/*
 * A002665  Continued fraction expansion of Lehmer's constant.
 * https://oeis.org/A002665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, nice, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=1, a(n) = (b(n-2)+b(n-3)+1)*a(n-2) for n >=
 *            3, where b = A002065: b(0)=0, b(k+1) = b(k)^2 + b(k) + 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n <= 2) return (seq_t)1;

    seq_t am2 = (seq_t)1;   /* a(k-2) */
    seq_t am1 = (seq_t)1;   /* a(k-1) */
    seq_t bp = (seq_t)0;    /* b(k-3) */
    seq_t bc = (seq_t)1;    /* b(k-2) */
    seq_t cur = (seq_t)1;

    for (idx_t k = 3; k <= n; k++) {
        cur = (bc + bp + (seq_t)1) * am2;
        am2 = am1;
        am1 = cur;
        if (k < n) {
            seq_t nb = bc * bc + bc + (seq_t)1;
            bp = bc;
            bc = nb;
        }
    }
    return cur;
}
