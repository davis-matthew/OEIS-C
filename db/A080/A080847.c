/*
 * A080847  a(n) = mu(n)+2, where mu is the Moebius function (A008683).
 * https://oeis.org/A080847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = mu(n) + 2, where mu is the Möbius function
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n == 0 (out of offset); return 0. */
    if (n == 0u)
        return (seq_t)0;

    int mu = 1;                /* Möbius value starts as 1. */
    idx_t temp = n;

    /* Trial division to detect prime factors and squares. */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1)          /* square factor -> mu = 0 */
                    return (seq_t)2;	/* mu = 0 ⇒ a(n) = 2 */
            }
            mu = -mu;                   /* toggle the sign for each distinct prime */
        }
    }

    if (temp > 1)                           /* remaining prime factor */
        mu = -mu;

    return (seq_t)(mu + 2u);                /* mu ∈ { -1, 0, 1 } → a(n) ∈ {1,2,3} */
}
