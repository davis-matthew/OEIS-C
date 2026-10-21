/*
 * A065882  Ultimate modulo 4: right-hand nonzero digit of n when written in
 *          base 4.
 * https://oeis.org/A065882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n mod 4 if n mod 4 != 0, otherwise a(n/4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Right‑hand non‑zero base‑4 digit of n. */

seq_t a(idx_t n) {
    if (n == 0)
        return 0;                      /* not used for offset 1, but safe */
    while ((n & 3) == 0)              /* while n is divisible by 4 */
        n >>= 2;                      /* divide by 4 */
    return (seq_t)(n & 3);            /* return n mod 4 (1,2, or 3) */
}
