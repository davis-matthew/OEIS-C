/*
 * A135416  a(n) = A036987(n)*(n+1)/2.
 * https://oeis.org/A135416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 9335, u32 n <= 9335, u64 n <= 9335,
 *            u128 n <= 9335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(2n)=0; a(2n+1)=2*a(n) */
seq_t a(idx_t n) {
    /* Zero for even indices */
    if ((n & 1ULL) == 0)
        return 0;
    seq_t val = 1;          /* a(1)=1 */
    while (n > 1) {
        /* Move to the parent odd index */
        n = (n - 1) >> 1;
        /* If the parent is even, entire branch is zero */
        if ((n & 1ULL) == 0)
            return 0;
        val <<= 1;          /* multiply current by 2 */
    }
    return val;
}
