/*
 * A160385  Number of nonzero digits in base-4 representation of n.
 * https://oeis.org/A160385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(4m)=a(m), a(4m+1)=a(4m+2)=a(4m+3)=1+a(m)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of non‑zero digits in the base‑4 representation of n. */
seq_t a(idx_t n) {
    seq_t count = 0;
    while (n > 0) {
        /* n % 4 is the lowest base‑4 digit; add 1 if it is non‑zero */
        count += (seq_t)((n & 3) != 0);
        /* n /= 4 : shift right by 2 bits */
        n >>= 2;
    }
    return count;
}
