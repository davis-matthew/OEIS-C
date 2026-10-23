/*
 * A018682  Divisors of 850.
 * https://oeis.org/A018682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 850 in ascending order (a(1)=1, a(2)=2, …, a(12)=850). */

seq_t a(idx_t n)
{
    static const seq_t divs[12] =
    {
        (seq_t)1,   (seq_t)2,   (seq_t)5,   (seq_t)10,
        (seq_t)17,  (seq_t)25,  (seq_t)34,  (seq_t)50,
        (seq_t)85,  (seq_t)170, (seq_t)425, (seq_t)850
    };

    if (n == 0) return (seq_t)0;        /* just in case */
    if (n <= 12) return divs[n-1];
    return (seq_t)0;                    /* outside the finite set */
}
