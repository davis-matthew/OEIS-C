/*
 * A019555  Smallest number whose cube is divisible by n.
 * https://oeis.org/A019555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = the smallest integer m such that m^3 is divisible by n.
 *            For each prime factor p^e of n, m contains the factor
 *            p^ceil(e/3)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n=0; return 0 as a safe value */
        return (seq_t)0;

    idx_t tmp = n;               /* work with a copy of n */
    seq_t res = (seq_t)1;        /* accumulating product of prime powers */

    /* Factor tmp by trial division.  This is adequate for n up to ~10^4
       (and faster for larger n, especially since the loop stops early). */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            int ceil_exp = (e + 2) / 3;          /* ceil(e/3) */
            for (int i = 0; i < ceil_exp; ++i)
                res *= (seq_t)p;
        }
    }

    /* If n has a remaining prime factor greater than sqrt(n) */
    if (tmp > 1) {
        /* Remaining prime has exponent 1 => ceil(1/3) = 1 */
        res *= (seq_t)tmp;
    }

    return res;
}
