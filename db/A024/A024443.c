/*
 * A024443  Expansion of 1/((1-x)(1-7x)(1-9x)(1-12x)).
 * https://oeis.org/A024443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*a(n-1) - 283*a(n-2) + 1011*a(n-3) - 756*a(n-4) a(0) =
 *            1, a(1) = 29, a(2) = 558, a(3) = 8986
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)558;
    if (n == 3) return (seq_t)8986;

    /* Use the recurrence to compute subsequent terms.
       We keep only the last four values to avoid allocating
       large buffers. */
    seq_t a0 = (seq_t)1;      /* a(n-4)  */
    seq_t a1 = (seq_t)29;     /* a(n-3)  */
    seq_t a2 = (seq_t)558;    /* a(n-2)  */
    seq_t a3 = (seq_t)8986;   /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute next = 29*a3 - 283*a2 + 1011*a1 - 756*a0.
           We avoid intermediate underflow by regrouping the
           positive and negative parts. */
        seq_t pos1 = (seq_t)29 * a3;
        seq_t pos2 = (seq_t)1011 * a1;
        seq_t posSum = pos1 + pos2;

        seq_t neg1 = (seq_t)756 * a0;
        seq_t neg2 = (seq_t)283 * a2;
        seq_t negSum = neg1 + neg2;

        next = posSum - negSum;

        /* Shift the window: a0←a1, a1←a2, a2←a3, a3←next */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
