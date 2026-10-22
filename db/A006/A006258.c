/*
 * A006258  Numerators of approximations to e.
 * https://oeis.org/A006258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) runs through the numerators of the convergents and
 *            intermediate fractions of the continued fraction of e = [2;
 *            1,2,1, 1,4,1, 1,6,1, ...]: for each partial quotient c(i)
 *            (c(0)=2, c(i)=2(i+1)/3 if i==2 mod 3, else 1) the terms are
 *            k*p(i-1) + p(i-2) for k = 1..c(i), with p(-2)=0, p(-1)=1,
 *            p(i)...
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pm2 = (seq_t)0;
    seq_t pm1 = (seq_t)1;
    idx_t count = 0;
    idx_t i = 0;
    for (;;) {
        idx_t q;
        if (i == 0) q = 2;
        else if (i % 3 == 2) q = 2 * (i + 1) / 3;
        else q = 1;
        seq_t val = pm2;
        idx_t k;
        for (k = 1; k <= q; k++) {
            val += pm1;
            count++;
            if (count == n) return val;
        }
        pm2 = pm1;
        pm1 = val;
        i++;
    }
}
