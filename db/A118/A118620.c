/*
 * A118620  Start with 1 and repeatedly reverse the digits and add 45 to get
 *          the next term.
 * https://oeis.org/A118620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = reverse(a(n-1)) + 45 with a(1)=1, a(2)=46; thereafter the
 *            sequence cycles with length 22
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)46;
    /* The cycle starts at a(3) and repeats every 22 steps. */
    static const seq_t cycle[22] = {
        (seq_t)109, (seq_t)946, (seq_t)694, (seq_t)541, (seq_t)190,
        (seq_t)136, (seq_t)676, (seq_t)721, (seq_t)172, (seq_t)316,
        (seq_t)658, (seq_t)901, (seq_t)154, (seq_t)496, (seq_t)739,
        (seq_t)982, (seq_t)334, (seq_t)478, (seq_t)919, (seq_t)964,
        (seq_t)514, (seq_t)460
    };
    return cycle[(n - 3) % 22];
}
