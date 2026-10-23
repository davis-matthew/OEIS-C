/*
 * A152118  a(n) = Product_{k=1..floor((n-1)/2)} (4 + 4*cos(k*Pi/n)^2).
 * https://oeis.org/A152118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A152118
   a(n) = Product_{k=1..floor((n-1)/2)} (4 + 4*cos(k*Pi/n)^2)
   Recurrence: a(n) = 6*a(n-2) - a(n-4)   for n > 4
   Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=5, a(4)=6   */

seq_t a(idx_t n)
{
    /* Base values for the first five indices. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)1;   /* a(2) */
    seq_t a3 = (seq_t)5;   /* a(3) */
    seq_t a4 = (seq_t)6;   /* a(4) */

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* Circular buffer of the last five terms.
       indices: (k % 5) holds a(k).   */
    seq_t buf[5] = {a0, a1, a2, a3, a4};

    for (idx_t k = 5; k <= n; ++k) {
        /* indices of the needed previous terms */
        idx_t idx_m2 = (k + 3) % 5;   /* (k-2) % 5 == (k+3) % 5   */
        idx_t idx_m4 = (k + 1) % 5;   /* (k-4) % 5 == (k+1) % 5   */
        seq_t curr = (seq_t)6 * buf[idx_m2] - buf[idx_m4];
        buf[k % 5] = curr;
    }

    return buf[n % 5];
}
