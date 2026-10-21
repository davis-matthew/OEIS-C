/*
 * A201632  If the sum of the squares of 4 consecutive numbers is a triangular
 *          number t(u), then a(n) is its index u.
 * https://oeis.org/A201632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A201632: a(n+4) = 34*a(n+2) - a(n) + 16
   with initial terms:
   a(1)=35, a(2)=83, a(3)=1203, a(4)=2835
   This recurrence produces all terms of the sequence.              */

seq_t a(idx_t n) {
    /* Offset: a(1) is the first term.  n is a 1‑based index. */
    if (n == 1) return (seq_t)35;
    if (n == 2) return (seq_t)83;
    if (n == 3) return (seq_t)1203;
    if (n == 4) return (seq_t)2835;

    /* t0 = a(i-4), t1 = a(i-3), t2 = a(i-2), t3 = a(i-1)   */
    seq_t t0 = 35, t1 = 83, t2 = 1203, t3 = 2835;
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)34 * t2 - t0 + (seq_t)16;  /* a(i) */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = next;
    }
    return t3;   /* a(n) */
}
