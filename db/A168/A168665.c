/*
 * A168665  a(n) = n^7*(n^8 + 1)/2.
 * https://oeis.org/A168665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7*(n^8 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 20, u128 n <= 387,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ (big‑int) implementation – all arithmetic uses seq_t operators */
    seq_t nn = (seq_t)n;
    seq_t pow7 = 1;
    for (int i = 0; i < 7; ++i) pow7 *= nn;   /* nn^7 */
    seq_t pow8 = pow7 * nn;                    /* nn^8 */
    seq_t val  = pow7 * (pow8 + 1);            /* nn^7 * (nn^8+1) */
    return val / 2;                            /* final division by 2 */
#else
    /* C (fixed‑width integer) implementation – use 128‑bit intermediates */
    unsigned __int128 nn = n;
    unsigned __int128 pow7 = 1;
    for (int i = 0; i < 7; ++i) pow7 *= nn;      /* nn^7 */
    unsigned __int128 pow8 = pow7 * nn;          /* nn^8 */
    unsigned __int128 val = pow7 * (pow8 + 1);   /* nn^7 * (nn^8+1) */
    val /= 2;                                   /* division by 2 */
    return (seq_t)val;                          /* cast back to seq_t */
#endif
}
