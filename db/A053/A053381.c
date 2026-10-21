/*
 * A053381  Maximal number of linearly independent smooth nowhere-zero vector
 *          fields on a (2n+1)-sphere.
 * https://oeis.org/A053381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = 2^c + 8*d – 1 where b = v₂(2(n+1)) (the exponent of 2 in
 *            2(n+1)) c = b mod 4, d = b / 4 (This follows the definition f(m)
 *            = 2^c + 8d – 1 for an m‑sphere; here m = 2n+1 so a(n) =
 *            f(2n+1).)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute exponent of 2 in 2(n+1). */
    unsigned v2_nplus1 = 0;
    /* We use uint64_t because idx_t is uint64_t. */
    uint64_t tmp = (uint64_t)(n + 1);
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        v2_nplus1++;
    }
    unsigned b = v2_nplus1 + 1;     /* exponent of 2 in 2(n+1) */
    unsigned c = b & 3;             /* b mod 4, 0 ≤ c ≤ 3 */
    unsigned d = b >> 2;            /* b / 4 */

    seq_t val = ((seq_t)1 << c) + ((seq_t)8 * d) - 1;
    return val;
}
