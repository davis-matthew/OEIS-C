/*
 * A060072  a(n) = (n^(n-1) - 1)/(n-1) for n>1, a(1) = 0.
 * https://oeis.org/A060072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^(n-1)-1)/(n-1) = Σ_{k=0}^{n-2} n^k , n>1, a(1)=0
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(1) = 0 */

    seq_t sum  = 1;                /* term n^0 = 1 */
    seq_t term = 1;                /* current n^k */

    /* The geometric series runs from k=0 up to k=n-2 inclusive.
       The loop below appends the next term for k = 1 … n-2. */
    for (idx_t k = 1; k < n - 1; ++k)
    {
#if defined(__SIZEOF_INT128__) && (SIZEOF_SEQ_T == 8)
        /* Use 128‑bit arithmetic to avoid intermediate overflow
           when seq_t is a 64‑bit unsigned integer. */
        unsigned __int128 prod = (unsigned __int128)term * (unsigned __int128)n;
        term = (seq_t)prod;
#else
        /* Relies on the natural capability of the seq_t type
           (e.g. 128‑bit unsigned or arbitrary precision). */
        term = term * (seq_t)n;
#endif
        sum += term;
    }

    return sum;
}
