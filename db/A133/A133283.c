/*
 * A133283  Numbers k such that 30*k^2 + 6 is a square.
 * https://oeis.org/A133283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A133283
 *  Recurrence:  a(1) = 1,  a(2) = 23,
 *               a(n) = 22*a(n-1) - a(n-2)   for n ≥ 3
 */

seq_t a(idx_t n)
{
    /* handle the initial values directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)23;

    /* iterate the linear recurrence */
    seq_t prev = (seq_t)1;   /* a(n-2) */
    seq_t curr = (seq_t)23;  /* a(n-1) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)22 * curr - prev; /* a(n) */
        prev = curr;
        curr = next;
    }
    return curr;
}
