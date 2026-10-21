/*
 * A098445  Coefficients of powers of q^5 in (q)_oo^5 == (q_5)^oo (mod 5).
 * https://oeis.org/A098445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n ≠ k(3k−1)/2 for any integer k; otherwise a(n) =
 *            (-1)^k (mod 5), i.e. 1 if k is even, 4 if k is odd
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of a 128‑bit number with binary search. */
static unsigned __int128 isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1, mid, sq;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)   lo = mid + 1;
        else          hi = (mid == 0 ? 0 : mid - 1);
    }
    return hi;  /* integer square root (floor). */
}

seq_t a(idx_t n)
{
    /* delta = 1 + 24*n  (must fit in 128 bits). */
    unsigned __int128 delta = (unsigned __int128)24 * (unsigned __int128)n + 1;

    /* Compute s = sqrt(delta).  If s*s != delta → no solution. */
    unsigned __int128 s = isqrt128(delta);
    if (s * s != delta)
        return (seq_t)0;                      /* not a pentagonal number */

    /* Determine the integer k such that n = k(3k-1)/2.  There can be
       at most one integer solution (k or -k).  Use the congruence
       condition: (1+s) ≡ 0 (mod 6) or (1−s) ≡ 0 (mod 6). */
    int k_even;
    if ((unsigned long long)((1 + s) % 6) == 0) {
        /* k = (1 + s) / 6 */
        unsigned __int128 k_val = (1 + s) / 6;
        k_even = ((k_val & 1) == 0);
    } else {
        /* k = (1 - s) / 6  (negative or zero) */
        signed __int128 k_val = (signed __int128)(1) -
                                (signed __int128)s;
        k_val /= 6;
        k_even = ((k_val & 1) == 0);              /* parity works for negative too */
    }

    /* Return 1 if k is even, 4 if k is odd. */
    return (seq_t)(k_even ? 1 : 4);
}
