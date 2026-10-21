/*
 * A228192  a(n) = A001850(n^2), where A001850 forms the central Delannoy
 *          numbers.
 * https://oeis.org/A228192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001850(n²) = central Delannoy number D(n²) Recurrence:
 *            (k+1)·D(k+1) = (6k+3)·D(k) – k·D(k–1) with D(0)=1, D(1)=3. The
 *            function handles any size of seq_t, using 128‑bit intermediate
 *            arithmetic when seq_t is 64‑bit to avoid overflow
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__SIZEOF_INT128__)
#define HAVE_INT128 1
#else
#define HAVE_INT128 0
#endif

seq_t a(idx_t n)
{
    /* Handle the trivial base cases */
    if (n == 0u)
        return (seq_t)1;

    /* Compute m = n² (fits in idx_t since n ≤ 10 000 in tests) */
    idx_t m = n * n;

    /* D(0) = 1, D(1) = 3 */
    seq_t d_prev2 = (seq_t)1;          /* D(0) */
    if (m == 0u)
        return d_prev2;

    seq_t d_prev1 = (seq_t)3;          /* D(1) */
    if (m == 1u)
        return d_prev1;

    /* Main loop: compute D(k+1) for k = 1 … m‑1 */
    for (idx_t k = 1u; k < m; ++k)
    {
#if HAVE_INT128
        if (sizeof(seq_t) == 8) {
            /* Use unsigned __int128 for safe multiplication */
            __int128_t term1 = (__int128_t)(6ull * k + 3u) * (__int128_t)d_prev1;
            __int128_t term2 = (__int128_t)k * (__int128_t)d_prev2;
            __int128_t numerator = term1 - term2;
            seq_t d_next = (seq_t)(numerator / (k + 1u));
            d_prev2 = d_prev1;
            d_prev1 = d_next;
        } else
#endif
        {
            /* Standard 64‑bit or arbitrary‑precision arithmetic */
            seq_t term1 = ((seq_t)(6 * k + 3)) * d_prev1;
            seq_t term2 = ((seq_t)k) * d_prev2;
            seq_t numerator = term1 - term2; /* exact division follows */
            seq_t d_next = numerator / (k + 1u);
            d_prev2 = d_prev1;
            d_prev1 = d_next;
        }
    }

    return d_prev1;  /* D(m) = a(n) */
}
