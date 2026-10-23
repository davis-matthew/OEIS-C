/*
 * A143101  Partial sums of A143097.
 * https://oeis.org/A143101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 + (n mod 3 == 0 ? 1 : 0)
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* defined only for n ≥ 1, but guard */
    seq_t a_val = (seq_t)n;
    seq_t b_val = (seq_t)n + 1;

    /* avoid overflow by dividing by 2 before multiplication */
    if ((a_val & 1) == 0) {
        a_val /= 2;
    } else {
        b_val /= 2;
    }
    seq_t result = a_val * b_val;

    /* add the extra 1 when n is divisible by 3 */
    if (n % 3 == 0) {
        result += 1;
    }
    return result;
}
