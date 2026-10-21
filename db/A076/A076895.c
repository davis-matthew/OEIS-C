/*
 * A076895  a(1) = 1, a(n) = n - a(ceiling(n/2)).
 * https://oeis.org/A076895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n - a(ceil(n/2))
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build a stack of the indices we need to unwind from a(1). */
    /* For any n ≤ 2^64 the depth is < 65, so a small fixed-size array suffices. */
    idx_t stack[64];
    int depth = 0;

    /* Push indices until we reach the base case a(1). */
    while (n > 1) {
        stack[depth++] = n;
        n = (n + 1) >> 1;  /* ceil(n/2) */
    }

    seq_t val = (seq_t)1;  /* a(1) */

    /* Unwind the stack: each level reverses a(n) = n - val. */
    for (int i = depth - 1; i >= 0; --i) {
        val = (seq_t)stack[i] - val;
    }
    return val;
}
