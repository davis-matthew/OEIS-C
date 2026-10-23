/*
 * A160384  Number of nonzero digits in the base-3 representation of n.
 * https://oeis.org/A160384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of nonzero digits in the base-3 representation of
 *            n
 * Range:     u8 n <= 9256, u16 n <= 9256, u32 n <= 9256, u64 n <= 9256,
 *            u128 n <= 9256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0; a(3m)=a(m); a(3m+1)=a(3m+2)=1+a(m). */
seq_t a(idx_t n) {
    seq_t count = 0;
    while (n > 0) {
        if ((n % 3) != 0) {
            count += 1;
        }
        n /= 3;
    }
    return count;
}
