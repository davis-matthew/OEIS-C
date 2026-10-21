/*
 * A082306  Expansion of e.g.f. (1+x)*exp(2*x)*cosh(x).
 * https://oeis.org/A082306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 1 + 3^(n-1)*(n + 3))/2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
/* C++ build – seq_t may be an arbitrary‑precision integer type. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* compute 3^(n-1) using seq_t arithmetic */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = (idx_t)1; i < n; ++i)
        pow3 *= (seq_t)3;

    /* numerator = (n + 1) + (n + 3) * 3^(n-1) */
    seq_t num = (seq_t)(n + (idx_t)1) + ((seq_t)(n + (idx_t)3) * pow3);

    /* division by 2 – the result is always an integer */
    return num / (seq_t)(idx_t)2;
}
#else
/* C11 build – seq_t is a built‑in unsigned integer type (<= 64 bits). 
   Use unsigned __int128 temporaries to avoid overflow during the
   intermediate multiplication. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    unsigned __int128 pow3 = (unsigned __int128)1;
    for (idx_t i = (idx_t)1; i < n; ++i)
        pow3 *= (unsigned __int128)3;

    unsigned __int128 numerator =
        (unsigned __int128)(n + (idx_t)1) +
        (unsigned __int128)(n + (idx_t)3) * pow3;

    /* divide by 2 – the numerator is guaranteed even */
    return (seq_t)(numerator >> 1);
}
#endif
