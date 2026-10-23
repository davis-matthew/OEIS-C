/*
 * A103974  Smaller sides (a) in (a,a,a+1)-integer triangle with integer area.
 * https://oeis.org/A103974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 5, a(3) = 65, a(n) = 15*a(n-1) - 15*a(n-2) +
 *            a(n-3) (for n ≥ 4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage expected by the OEIS build system. */
seq_t a(idx_t n)
{
    /* Guard against an undefined index; not used by the tests. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Small fixed‐size cases: direct answers. */
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)5;
    }
    if (n == 3) {
        return (seq_t)65;
    }

    /* Common recurrence:  a(k+3) = 15*a(k+2) – 15*a(k+1) + a(k)   (k ≥ 0).
       The following iterative loop produces a(n) using only the last
       three values – O(1) extra memory and O(n) time. */
    seq_t a0 = (seq_t)1;   /* a(1)  */
    seq_t a1 = (seq_t)5;   /* a(2)  */
    seq_t a2 = (seq_t)65;  /* a(3)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)15 * a2 - (seq_t)15 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
