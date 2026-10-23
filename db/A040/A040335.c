/*
 * A040335  Continued fraction for sqrt(354).
 * https://oeis.org/A040335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of partial quotients for the continued fraction of sqrt(354).
   a(0) = 18, and for n > 0:
   a(n) = c[(n-1) % 10] where c = [1,4,2,2,18,2,2,4,1,36]. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)18;
    static const seq_t cycle[10] = {
        (seq_t)1, (seq_t)4, (seq_t)2, (seq_t)2,
        (seq_t)18, (seq_t)2, (seq_t)2, (seq_t)4,
        (seq_t)1, (seq_t)36
    };
    return cycle[(n - 1) % 10];
}
