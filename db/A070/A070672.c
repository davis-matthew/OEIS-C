/*
 * A070672  Smallest m in range 2..n-1 such that m^7 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 529, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute smallest m in [2,n‑1] such that m^7 ≡ 1 (mod n),
   or 1 if no such m exists.  */
seq_t a(idx_t n)
{
    /* For n = 1 or 2 no m in the required range; return 1. */
    if (n <= 2)
        return 1;

    /* Cast modulus to seq_t once to avoid repeated casts. */
    seq_t mod = (seq_t)n;

    for (idx_t m = 2; m < n; ++m)
    {
        seq_t x = (seq_t)m;

        /* Compute m^7 modulo n using repeated multiplication. */
        seq_t t = (x * x) % mod;   /* m^2 */
        t = (t * x) % mod;         /* m^3 */
        t = (t * t) % mod;         /* m^6 */
        t = (t * x) % mod;         /* m^7 */

        if (t == 1)
            return (seq_t)m;
    }

    /* No m found; return 1 as specified. */
    return 1;
}
