/*
 * A147848  Number (up to isomorphism) of groups of order 2n that have Z/nZ as
 *          a subgroup (that is, that have an element of order n).
 * https://oeis.org/A147848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A147848: Number (up to isomorphism) of groups of order 2n that
   have Z/nZ as a subgroup (an element of order n).
   The sequence is multiplicative.
   For each n = Π p^e:
     - If p = 2:  a(2^1)=2, a(2^2)=4, a(2^e)=6 for e≥3
     - If p is odd: a(p^e)=2
   Therefore: a(n) = Π a(p^e) over all prime powers of n. */

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;                /* a(1) = 1 */

    seq_t result = 1;
    idx_t v = n;

    /* handle factor 2 */
    if ((v & 1) == 0) {
        idx_t e = 0;
        while ((v & 1) == 0) {
            v >>= 1;
            ++e;
        }
        if (e == 1)
            result *= 2;
        else if (e == 2)
            result *= 4;
        else
            result *= 6;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= v / p; p += 2) {
        if (v % p == 0) {
            /* count exponent e but we do not need its value */
            while (v % p == 0)
                v /= p;
            result *= 2;          /* a(p^e) = 2 for odd p */
        }
    }

    /* remaining prime > sqrt(original) */
    if (v > 1)          /* a(v^1) = 2 for odd prime */
        result *= 2;

    return result;
}
