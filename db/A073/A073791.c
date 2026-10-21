/*
 * A073791  Replace 4^k with (-4)^k in base 4 expansion of n.
 * https://oeis.org/A073791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(0)=0, a(4k+m) = -4*a(k)+m (0≤m<4)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Replace 4^k with (-4)^k in the base‑4 expansion of n. */
seq_t a(idx_t n) {
    /* Base case: digits 0 … 3 are unchanged. */
    if (n < 4)
        return (seq_t)n;

    /* Recursively evaluate a(k) where k = n/4, then apply the recurrence. */
    return -(seq_t)4 * a(n / 4) + (seq_t)(n % 4);
}
