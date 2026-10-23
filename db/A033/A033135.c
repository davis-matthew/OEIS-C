/*
 * A033135  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,1,0.
 * https://oeis.org/A033135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*a(n-1) + a(n-3) - 8*a(n-4) Initial terms: a(1)=1,
 *            a(2)=9, a(3)=72, a(4)=577
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)72;
    if (n == 4) return (seq_t)577;

    /* arr[0] = a(i-1), arr[1] = a(i-2), arr[2] = a(i-3), arr[3] = a(i-4) */
    seq_t arr[4];
    arr[0] = (seq_t)577; /* a(4) */
    arr[1] = (seq_t)72;  /* a(3) */
    arr[2] = (seq_t)9;   /* a(2) */
    arr[3] = (seq_t)1;   /* a(1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)8 * arr[0] + arr[2] - (seq_t)8 * arr[3];
        /* shift for next iteration */
        arr[3] = arr[2];
        arr[2] = arr[1];
        arr[1] = arr[0];
        arr[0] = next;
    }
    return arr[0];
}
