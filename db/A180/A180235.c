/*
 * A180235  Demi-tribonacci numbers (rounding up): a(0)=a(1)=0, a(2)=2; a(n) =
 *          ceiling( (a(n-1)+a(n-2)+a(n-3))/2 ).
 * https://oeis.org/A180235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil((a(n-1)+a(n-2)+a(n-3))/2), a(0)=a(1)=0, a(2)=2
 * Range:     u8 n <= 22, u16 n <= 49, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial conditions directly. */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;

    /* Iteratively compute a(n) for n >= 3. */
    seq_t a0 = 0;   /* a(n-3) */
    seq_t a1 = 0;   /* a(n-2) */
    seq_t a2 = 2;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = a2 + a1 + a0;                /* a(n-1)+a(n-2)+a(n-3) */
        seq_t next = (sum >> 1) + (sum & (seq_t)1); /* ceil(sum/2) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
