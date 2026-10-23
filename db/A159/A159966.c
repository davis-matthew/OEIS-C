/*
 * A159966  Lodumo_4 of A102370 (sloping binary numbers).
 * https://oeis.org/A159966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, less, base
 * Formula:   a(n) = n, if n even; a(n) = n + 2 if n ≡ 1 (mod 4); a(n) = n – 2
 *            if n ≡ 3 (mod 4)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1ULL) == 0ULL)
        return (seq_t)n;

    /* odd indices */
    seq_t half = ((seq_t)n + 1ULL) / 2;          /* e = (n+1)/2 */
    /* (-1)^e is -1 when e is odd, +1 when e is even */
    if ((half & 1ULL) == 0ULL)                  /* e even → a(n) = n - 2 */
        return (seq_t)n - 2;
    else                                        /* e odd  → a(n) = n + 2 */
        return (seq_t)n + 2;
}
