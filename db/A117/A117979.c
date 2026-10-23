/*
 * A117979  a(n)=A117611(9n).
 * https://oeis.org/A117979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A117611(9n) = Σ_{k=0}^{9n} Legendre(C(9n,k),3) * 10^k
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Legendre(C,3) = 0 if 3 | C, = 1 if C ≡ 1 (mod 3), = -1 if C ≡ 2 (mod 3).  */

static const int binom_mod3[3][3] = {
    /* C(n,k) mod 3 for n,k ∈ {0,1,2} */
    {1,0,0},          /* n = 0   */
    {1,1,0},          /* n = 1   */
    {1,2,1}           /* n = 2   */
};

seq_t a(idx_t n) {
    idx_t m = 9 * n;                 /* m = 9n */

    /* base‑3 digits of m (least significant first) */
    idx_t tmp = m;
    int digits[20];
    int dlen = 0;
    if (tmp == 0) {
        digits[dlen++] = 0;
    } else {
        while (tmp) {
            digits[dlen++] = tmp % 3;
            tmp /= 3;
        }
    }

    seq_t pos = 0;      /* sum of positive contributions */
    seq_t neg = 0;      /* sum of negative contributions   */
    seq_t power = 1;    /* current 10^k, starts at 10^0 */

    for (idx_t k = 0; k <= m; ++k) {
        idx_t kk = k;
        int coeff = 1;  /* product of binom mod 3, starts at 1 */

        for (int i = 0; i < dlen; ++i) {
            int mk = kk % 3;
            int nk = digits[i];
            if (mk > nk) { coeff = 0; break; }

            /* multiply by C(nk, mk) mod 3, skip if mk==0 (binom=1) */
            if (mk) {
                coeff = (coeff * binom_mod3[nk][mk]) % 3;
            }
            kk /= 3;
        }

        if (coeff == 1)      pos += power;
        else if (coeff == 2) neg += power;   /* coeff == 2 ⇔ Legendre symbol = -1 */

        power *= (seq_t)10;
    }

    /* The sequence is always non‑negative; pos ≥ neg for all n. */
    return pos - neg;
}
