/*
 * A186813  a(n) = n if n odd, a(2n) = 3n if n odd, a(4n) = 2n.
 * https://oeis.org/A186813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A186813
 * Formula:
 *   a(n) = n                        if n is odd
 *   a(n) = 3*n/2                    if n ≡ 2 (mod 4)
 *   a(n) = n/2                      if n ≡ 0 (mod 4)
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Count exponent of 2 in n (v2). */
    unsigned v2 = 0;
    idx_t tmp = n;
    /* tmp is guaranteed non‑zero here. */
    while ((tmp & 1ULL) == 0) {
        v2++;
        tmp >>= 1;
    }

    seq_t sn = (seq_t)n;

    if (v2 == 0)
        return sn;          /* odd n */
    if (v2 == 1)
        return sn * 3 / 2;  /* n ≡ 2 mod 4 */
    /* v2 >= 2 : n ≡ 0 mod 4 */
    return sn / 2;
}
