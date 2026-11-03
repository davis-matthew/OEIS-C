/*
 * A006561  Number of intersections of diagonals in the interior of a regular
 *          n-gon.
 * https://oeis.org/A006561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   for n >= 3, a(n) = binomial(n,4) + (-5*n^3 + 45*n^2 - 70*n +
 *            24)*delta(2,n)/24 - (3*n/2)*delta(4,n) + (-45*n^2 +
 *            262*n)*delta(6,n)/6 + 42*n*delta(12,n) + 60*n*delta(18,n) +
 *            35*n*delta(24,n) - 38*n*delta(30,n) - 82*n*delta(42,n) -
 *            330*n*delta(60,n) - 144*n*delta(84,n) - 96*n*delta(90,n) -
 *            144*n*de...
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 60 terms through n = 9545 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* binomial(n,4) built from exact native factors so fixed-width wrap stays consistent */
static seq_t a006561_binom4(idx_t n) {
    uint64_t f[4];
    int i;
    for (i = 0; i < 4; i++) f[i] = (uint64_t)n - (uint64_t)i;
    for (i = 0; i < 4; i++) if (f[i] % 3 == 0) { f[i] /= 3; break; }
    for (i = 0; i < 4; i++) if (f[i] % 4 == 0) { f[i] /= 4; break; }
    for (i = 0; i < 4; i++) if (f[i] % 2 == 0) { f[i] /= 2; break; }
    {
        seq_t r = (seq_t)f[0];
        r *= (seq_t)f[1];
        r *= (seq_t)f[2];
        r *= (seq_t)f[3];
        return r;
    }
}

seq_t a(idx_t n) {
    long long m, corr = 0;
    seq_t result;
    if (n < 4) return 0;

    m = (long long)n;
    if (n % 2 == 0) corr += (-5LL*m*m*m + 45LL*m*m - 70LL*m + 24) / 24;
    if (n % 4 == 0) corr -= (3*m) / 2;
    if (n % 6 == 0) corr += (-45LL*m*m + 262LL*m) / 6;
    if (n % 12 == 0) corr += 42*m;
    if (n % 18 == 0) corr += 60*m;
    if (n % 24 == 0) corr += 35*m;
    if (n % 30 == 0) corr -= 38*m;
    if (n % 42 == 0) corr -= 82*m;
    if (n % 60 == 0) corr -= 330*m;
    if (n % 84 == 0) corr -= 144*m;
    if (n % 90 == 0) corr -= 96*m;
    if (n % 120 == 0) corr -= 144*m;
    if (n % 210 == 0) corr -= 96*m;

    result = a006561_binom4(n);
    if (corr >= 0)
        result += (seq_t)(unsigned long long)corr;
    else
        result -= (seq_t)(unsigned long long)(-corr);
    return result;
}
