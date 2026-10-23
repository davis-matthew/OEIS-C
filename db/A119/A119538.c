/*
 * A119538  Fixed point of the morphism a -> {a, a + 1, 2a + 2} beginning with
 *          0.
 * https://oeis.org/A119538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = value obtained by iterating the morphism
 * a -> {a, a+1, 2a+2} on 0.
 *
 * Equivalent: write (n-1) in base 3 and process its digits from
 * most to least significant:
 *   start value v = 0
 *   if digit == 0: v stays the same
 *   if digit == 1: v += 1
 *   if digit == 2: v = 2*v + 2
 *
 * This yields a(1)=0, a(2)=1, a(3)=2, a(4)=1, a(5)=2, a(6)=4, ... */

seq_t a(idx_t n) {
    idx_t m = n - 1;            /* n >= 1, so m is valid */
    idx_t p = 1;
    /* find the largest power of 3 <= m (or 1 if m==0) */
    while (p <= m / 3) {
        p *= 3;
    }

    seq_t v = 0;
    while (p > 0) {
        idx_t digit = m / p;           /* 0, 1 or 2 */
        if (digit == 1) {
            v += 1;
        } else if (digit == 2) {
            v = (v << 1) + 2;          /* v = 2*v + 2 */
        }
        m -= digit * p;
        p /= 3;
    }
    return v;
}
