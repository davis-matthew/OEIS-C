/*
 * A037677  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,2,3.
 * https://oeis.org/A037677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A037677  
   a(1) = 1, a(2) = 7, a(3) = 51, a(4) = 360  
   Recurrence: a(n) = 8*a(n-1) - 8*a(n-2) + 8*a(n-3) - 7*a(n-4)   (n ≥ 5) */

seq_t a(idx_t n)
{
    /* Handle the initial 4 terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)51;
    if (n == 4) return (seq_t)360;
    /* Compute onward with the 4‑term recurrence. */
    seq_t a1 = (seq_t)1;   /* a(n-4) */
    seq_t a2 = (seq_t)7;   /* a(n-3) */
    seq_t a3 = (seq_t)51;  /* a(n-2) */
    seq_t a4 = (seq_t)360; /* a(n-1) */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)8 * a4 - (seq_t)8 * a3 + (seq_t)8 * a2 - (seq_t)7 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
