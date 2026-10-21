/*
 * A206590  Number of solutions (n,k) of k^3=n^3 (mod n), where 1<=k<n.
 * https://oeis.org/A206590
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1) / ∏_p p^{ceil(v_p(n)/3)} ) for n ≥ 2
 * Range:     u8 n <= 1883, u16 n <= 9457, u32 n <= 9457, u64 n <= 9457,
 *            u128 n <= 9457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where n = ∏_p p^{v_p(n)} is the prime factorisation of n. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t temp = n;
    seq_t s = 1;                  /* s = ∏_p p^{ceil(e/3)} */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            int ceil_e = (e + 2) / 3;   /* ceil(e/3) */
            for (int i = 0; i < ceil_e; ++i)
                s *= p;
        }
    }

    if (temp > 1) {                /* remaining prime factor (exponent = 1) */
        s *= temp;
    }

    return (seq_t)((n - 1) / s);
}
