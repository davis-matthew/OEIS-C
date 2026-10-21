/*
 * A211517  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          w^3=(x+y)^2.
 * https://oeis.org/A211517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k=1}^{\lfloor\sqrt{n}\rfloor}
 *            \max\bigl(0,\;\min(n,\;k^3-1)-\max(1,\;k^3-n)+1\bigr)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  w must be a perfect square k^2, and x+y = k^3.  Count ordered
    pairs (x,y) with 1≤x,y≤n.  That yields the above formula. */

seq_t a(idx_t n)
{
    seq_t total = 0;
    const __int128 two_n = (__int128)2 * n;

    for (idx_t k = 1;; ++k) {
        /* k^2 <= n must hold (w <= n). */
        idx_t k2 = k * k;
        if (k2 > n)
            break;

        /* s = k^3, computed in 128 bits to avoid overflow. */
        __int128 s = (__int128)k2 * k;

        /* For larger k, s only increases, so if it already exceeds
           2*n no further triples can exist. */
        if (s > two_n)
            break;

        /* Compute the range of x giving 1≤y≤n. */
        idx_t L; /* lower bound for x */
        if (s > n)              /* s > n ⇒ y = s - x ≤ n ⇒ x ≥ s-n */
            L = (idx_t)(s - n);
        else
            L = 1;

        idx_t U; /* upper bound for x */
        if (s <= n)              /* s-1 < n ⇒ U = s-1 */
            U = (idx_t)(s - 1);
        else
            U = n;

        if (L <= U)
            total += (seq_t)(U - L + 1);
    }

    return total;
}
