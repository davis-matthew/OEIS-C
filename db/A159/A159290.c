/*
 * A159290  A generalized Jacobsthal sequence.
 * https://oeis.org/A159290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + (n%2==0 ? 3 : -1), a(0) = 3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute a(n) by iterating the recurrence. */
    seq_t result = (seq_t)3;   /* a(0) */
    if (n == 0) {
        return result;
    }
    for (idx_t i = 1; i <= n; ++i) {
        /* Multiply previous term by 2. */
        result = result << 1;   /* result = 2 * result */
        /* Apply the parity-dependent add/subtract. */
        if ((i & 1) == 0) {                /* even i */
            result += (seq_t)3;            /* +3 */
        } else {                          /* odd i */
            result -= (seq_t)1;            /* -1 */
        }
    }
    return result;
}
