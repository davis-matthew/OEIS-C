/*
 * A091527  a(n) = ((3*n)!/n!^2)*(Gamma(1+n/2)/Gamma(1+3n/2)).
 * https://oeis.org/A091527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ((3*n)!/(n!)^2) * (Gamma(1+n/2)/Gamma(1+3n/2))
 * Recurrence: a(0)=1, a(1)=4,
 *             for n>=2: a(n) = a(n-2) * 12*(3*n-1)*(3*n-5) / (n*(n-1))
 *
 * The implementation uses the recurrence.  For builds where `seq_t` is
 * 8‑bytes or less (e.g. uint64_t) the intermediate multiplication is
 * performed in an unsigned __int128 to avoid overflow; otherwise the
 * arithmetic is performed directly on the `seq_t` type (which may be an
 * arbitrary‑precision class).
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t cur;

    for (idx_t k = 2; k <= n; ++k) {
        /* use 128‑bit temporary only when seq_t fits in 8 bytes */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 tmp = (unsigned __int128)prev2 * 12 *
                                   (unsigned __int128)(3*k-1) *
                                   (unsigned __int128)(3*k-5);
            tmp /= (unsigned __int128)(k*(k-1));
            cur = (seq_t)tmp;
        } else {
            /* arbitrary‑precision path – operate directly on seq_t */
            cur = prev2 * (seq_t)12;
            cur = cur * (seq_t)(3*k-1);
            cur = cur * (seq_t)(3*k-5);
            seq_t denom = (seq_t)k * (seq_t)(k-1);
            cur = cur / denom;
        }

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
