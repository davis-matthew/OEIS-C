/*
 * A046178  Indices of pentagonal numbers that are also hexagonal.
 * https://oeis.org/A046178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046178: Indices of pentagonal numbers that are also hexagonal
 * Recurrence:
 *   a(1) = 1
 *   a(2) = 165
 *   a(n) = 194·a(n‑1) – a(n‑2) – 32 for n ≥ 3
 * This implementation evaluates the recurrence iteratively.
 * For any n the value is computed in time O(n) using only integer
 * arithmetic.  Because seq_t can be an arbitrary‑precision class,
 * the expression `194 * a_curr` uses Seq_t’s multiplication and
 * therefore never overflows; if seq_t is a native integral type the
 * multiplication is performed in that integer type and overflows only
 * for n beyond the range where the result fits, which is the behaviour
 * required by the specification.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined, but safe return */

    /* Handle small indices directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)165;

    /* Iterative recurrence. */
    seq_t a_prev = (seq_t)1;   /* a(1) */
    seq_t a_curr = (seq_t)165; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_next = (seq_t)194 * a_curr - a_prev - (seq_t)32;
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}
