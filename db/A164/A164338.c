/*
 * A164338  Conway's creeper sequence.
 * https://oeis.org/A164338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=12334444, a(2)=55667777, a(n+2)=10*a(n)-9996 (n odd),
 *            a(n+2)=10*a(n)-9993 (n even)
 * Range:     u8 none, u16 none, u32 n <= 5, u64 n <= 25, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)12334444;
    }
    if (n == 2) {
        return (seq_t)55667777;
    }
    seq_t prev2 = (seq_t)12334444; /* a(1) */
    seq_t prev1 = (seq_t)55667777; /* a(2) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i) {
        /* (i-2) is the n used in the recurrence a(n+2) */
        if (((i - 2) & 1) != 0) {      /* n is odd */
            curr = ((seq_t)10 * prev2) - (seq_t)9996;
        } else {                       /* n is even */
            curr = ((seq_t)10 * prev2) - (seq_t)9993;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
