/*
 * A128961  a(n) = (n^3 - n)*3^n.
 * https://oeis.org/A128961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (n^3 - n)*3^n ;
   Recurrence: a(n) = 12*a(n-1) - 54*a(n-2) + 108*a(n-3) - 81*a(n-4);
   Initial values: a(1)=0, a(2)=54, a(3)=648, a(4)=4860. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* offset is 1 */

    /* base values */
    seq_t a1 = (seq_t)0;       /* a(1) */
    seq_t a2 = (seq_t)54;      /* a(2) */
    seq_t a3 = (seq_t)648;     /* a(3) */
    seq_t a4 = (seq_t)4860;    /* a(4) */

    /* return if we are in the beginning */
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* iterate using the 4‑term recurrence */
    seq_t curr;
    for (idx_t i = 5; i <= n; ++i) {
        curr = (seq_t)12 * a4
               - (seq_t)54 * a3
               + (seq_t)108 * a2
               - (seq_t)81 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = curr;
    }
    return a4;
}
