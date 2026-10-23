/*
 * A174910  Partial sums of A028835.
 * https://oeis.org/A174910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 18*⌊n/4⌋² – ⌊n/4⌋ + 9*⌊n/4⌋*(n mod 4) + base_prefix[n mod
 *            4] where base_prefix = {0, 2, 5, 10} (prefixed partial sums of
 *            one cycle)
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1‑based; a(0) returns 0 to be safe */
    idx_t q = n / 4;     /* floor(n/4) */
    idx_t r = n % 4;     /* n mod 4 */

    seq_t Q = (seq_t)q;  /* cast to seq_t for arithmetic */

    /* 18*q² – q  (q = floor(n/4)) */
    seq_t res = (seq_t)18 * Q * Q - Q;

    /* + 9*q*r */
    res += (seq_t)9 * Q * (seq_t)r;

    /* + base_prefix[r] – using the first 4 partial sums of a cycle */
    static const seq_t base_prefix[4] = { (seq_t)0, (seq_t)2, (seq_t)5, (seq_t)10 };
    res += base_prefix[r];

    return res;
}
