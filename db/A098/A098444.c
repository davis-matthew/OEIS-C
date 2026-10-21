/*
 * A098444  Expansion of 1/sqrt(1-6x-11x^2).
 * https://oeis.org/A098444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3 Recurrence: n*a(n) = 3*(2*n-1)*a(n-1) +
 *            11*(n-1)*a(n-2) (for n>=2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t ii = i;                     /* cast to idx_t again for clarity */
        seq_t next;

        /* If seq_t fits in 8 bytes or less, use a wider intermediate
           type (unsigned __int128) to avoid overflow. */
        if (sizeof(seq_t) <= 8)
        {
            unsigned __int128 t1 = (unsigned __int128)3 *
                                   (unsigned __int128)(2 * ii - 1) *
                                   (unsigned __int128)a1;

            unsigned __int128 t2 = (unsigned __int128)11 *
                                   (unsigned __int128)(ii - 1) *
                                   (unsigned __int128)a0;

            unsigned __int128 sum = t1 + t2;
            next = (seq_t)(sum / (unsigned __int128)ii);
        }
        else
        {
            /* seq_t is large enough that normal operations will
               not overflow (e.g., 128‑bit or arbitrary‑precision). */
            seq_t t1 = (seq_t)3 * (seq_t)(2 * ii - 1) * a1;
            seq_t t2 = (seq_t)11 * (seq_t)(ii - 1) * a0;
            next = (t1 + t2) / ii;
        }

        a0 = a1;
        a1 = next;
    }
    return a1;
}
