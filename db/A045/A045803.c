/*
 * A045803  3-ish numbers (end in 17, 19, 31, 33).
 * https://oeis.org/A045803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A045803: 3-ish numbers
   Recurrence: a(n) = a(n-1) + a(n-4) - a(n-5),  n >= 6
   Initial values:
      a(1) = 17,  a(2) = 19,  a(3) = 31,  a(4) = 33,  a(5) = 117
*/
seq_t a(idx_t n) {
    /* Handle the first five terms directly. */
    switch (n) {
        case 1: return (seq_t)17;
        case 2: return (seq_t)19;
        case 3: return (seq_t)31;
        case 4: return (seq_t)33;
        case 5: return (seq_t)117;
        default: break;
    }

    /* Prepare the first five terms for the recurrence. */
    seq_t a1 = (seq_t)17;   /* a(n-5)   */
    seq_t a2 = (seq_t)19;   /* a(n-4)   */
    seq_t a3 = (seq_t)31;   /* a(n-3)   */
    seq_t a4 = (seq_t)33;   /* a(n-2)   */
    seq_t a5 = (seq_t)117;  /* a(n-1)   */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(n) = a(n-1) + a(n-4) - a(n-5)  ->  a5 + a2 - a1 */
        seq_t next = a5 + a2 - a1;

        /* shift the window: a1 <- a2, a2 <- a3, a3 <- a4, a4 <- a5, a5 <- next */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
