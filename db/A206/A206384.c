/*
 * A206384  Number of n X 3 0..6 arrays with no element equal to another
 *          within a city block distance of two, and new values 0..6
 *          introduced in row major order.
 * https://oeis.org/A206384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 21*a(n-1) + 131*a(n-2) – 637*a(n-3) + 486*a(n-4) for n>6
   with a(1)=1, a(2)=4, a(3)=55, a(4)=1368, a(5)=34442, a(6)=868994 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined for n=0, harmless */

    if (n <= 6) {
        static const seq_t init[7] = {
            (seq_t)0,          /* dummy for 1‑based indexing */
            (seq_t)1, (seq_t)4, (seq_t)55,
            (seq_t)1368, (seq_t)34442, (seq_t)868994
        };
        return init[n];
    }

    /* a1 = a(i-4), a2 = a(i-3), a3 = a(i-2), a4 = a(i-1) for the current i */
    seq_t a1 = (seq_t)55,   /* a(3)   */
          a2 = (seq_t)1368, /* a(4)   */
          a3 = (seq_t)34442,/* a(5)   */
          a4 = (seq_t)868994/* a(6)   */

    ;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 21 * a4 + 131 * a3 - 637 * a2 + 486 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
