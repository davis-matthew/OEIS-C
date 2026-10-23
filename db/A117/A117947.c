/*
 * A117947  T(n,k) = L(C(n,k)/3) where L(j/p) is the Legendre symbol of j and
 *          p.
 * https://oeis.org/A117947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = Legendre symbol (C(r, k) / 3), where r and k satisfy * n
 *            = r*(r+1)/2 + k and 0 ≤ k ≤ r. The Legendre symbol equals * 0 if
 *            3 divides C(r, k), 1 if C(r, k) ≡ 1 (mod 3), and -1 if C(r, k) ≡
 *            2 (mod 3)
 * Range:     i8 n <= 119, i16 n <= 119, i32 n <= 119, i64 n <= 119,
 *            i128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow for very large n. */
    while ( (__uint128_t)(r + 1) * (r + 2) <= (__uint128_t)2 * n )
        ++r;

    /* Starting index of this row. */
    idx_t start = (idx_t)((__uint128_t)r * (r + 1) / 2);
    idx_t k = n - start;

    /* Compute Legendre symbol of C(r, k) mod 3 using Lucas' theorem. */
    idx_t rr = r;
    idx_t kk = k;
    int prod = 1;   /* product modulo 3 */

    while (rr || kk) {
        uint8_t ni = (uint8_t)(rr % 3);
        uint8_t ki = (uint8_t)(kk % 3);
        if (ki > ni)        /* binomial coefficient divisible by 3 */
            return (seq_t)0;

        /* Small binomial value modulo 3 for digits 0..2. */
        int val;
        if (ki == 0 || ki == ni)      /* C(0,0), C(1,0), C(1,1), C(2,0), C(2,2) */
            val = 1;
        else                           /* only remaining case is (n_i, k_i) = (2,1) */
            val = 2;                  /*  C(2,1)=2 ≡ 2 (mod 3) */

        prod = (prod * val) % 3;
        rr /= 3;
        kk /= 3;
    }

    if (prod == 1)
        return (seq_t)1;
    else
        return (seq_t)-1;   /* prod == 2  →  -1 */
}
