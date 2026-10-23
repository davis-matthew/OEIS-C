/*
 * A011605  Legendre symbol (n,107).
 * https://oeis.org/A011605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,107); equivalently a(n) = n^53 (mod
 *            107), with 0 returned if 107 | n, 1 if n^53 ≡ 1 (mod 107), and
 *            -1 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce the index modulo 107 */
    uint64_t nmod = (uint64_t)n % 107ULL;

    /* 107 | n  → 0 */
    if (nmod == 0ULL)
        return 0;

    /* Binary exponentiation of nmod^53 mod 107 */
    uint64_t base = nmod;     /* nmod fits in 8 bits, but use 64 for safety   */
    uint64_t exp  = 53ULL;    /* (p-1)/2 for prime 107                       */
    uint64_t res  = 1ULL;

    while (exp != 0ULL) {
        if (exp & 1ULL)
            res = (res * base) % 107ULL;
        base = (base * base) % 107ULL;
        exp >>= 1ULL;
    }

    /* Map results 1 → 1, 106 → -1 (since -1 ≡ 106 mod 107) */
    if (res == 1ULL)
        return 1;
    else
        return (seq_t)-1;
}
