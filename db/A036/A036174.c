/*
 * A036174  Log base 2 (n) mod 67.
 * https://oeis.org/A036174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the smallest non‑negative integer k such that 2^k ≡ n
 *            (mod 67). For n ≡ 0 (mod 67) the sequence is defined to return
 *            0. The function computes the discrete logarithm base 2 modulo 67
 *            by precomputing the powers of 2 modulo 67 once and searching the
 *            result. This satisfies the idempotent and m...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* pre‑compute 2^k (mod 67) for k = 0..65 */
    static seq_t pow2[66];
    static int initialized = 0;
    if (!initialized)
    {
        pow2[0] = 1 % 67U;          /* 2^0 mod 67 = 1 */
        for (idx_t i = 1; i < 66; ++i)
            pow2[i] = (pow2[i-1] * 2) % 67U;
        initialized = 1;
    }

    if (n == 0)                         /* undefined; return 0 for safety */
        return 0;

    seq_t r = (seq_t)(n % 67U);         /* residue modulo 67 */
    if (r == 0)
        return 0;                       /* no solution if n divisible by 67 */

    /* search for r in the precomputed table */
    for (idx_t i = 0; i < 66; ++i)
        if (pow2[i] == r)
            return (seq_t)i;            /* k such that 2^k ≡ n (mod 67) */

    return 0; /* unreachable: every r in 1..66 appears exactly once */
}
