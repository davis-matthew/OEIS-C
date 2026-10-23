/*
 * A022395  Fibonacci sequence beginning 1, 25.
 * https://oeis.org/A022395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + a(n-2)   (Fibonacci recurrence)
   a(0) = 1
   a(1) = 25
   This implementation iterates from 2 up to n and returns the
   computed value.  It works for any seq_t type (builtin integer or
   arbitrary‑precision class) and for any idx_t type (uint64_t). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)25;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)25;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
