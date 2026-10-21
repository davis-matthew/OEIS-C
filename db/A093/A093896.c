/*
 * A093896  Least positive k such that n^n divides k!.
 * https://oeis.org/A093896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002034(n^n) * Implementation: for each prime factor p^a
 *            of n, compute the exponent * E = n*a required of that prime in
 *            k!. Find the least k such that * Legendre(k, p) >= E by binary
 *            search. The answer is the maximum * such k over all prime
 *            factors of n. Special case n = 1 => a(1) = 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t legendre(seq_t k, seq_t p)
{
    seq_t cnt = 0;
    while (k) {
        k /= p;
        cnt += k;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Factorise n by trial division */
    idx_t nn = n;
    seq_t best = 0;

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= nn; ++p) {
        if (nn % p == 0) {
            /* count exponent a of p in n */
            idx_t a_exp = 0;
            while (nn % p == 0) {
                ++a_exp;
                nn /= p;
            }

            /* required exponent of p in k! */
            seq_t need = (seq_t)n * (seq_t)a_exp;

            /* binary search for minimal k */
            seq_t lo = 1;
            seq_t hi = need * (seq_t)p;      /* upper bound: p*need */

            while (lo < hi) {
                seq_t mid = (lo + hi) / 2;
                if (legendre(mid, (seq_t)p) >= need)
                    hi = mid;
                else
                    lo = mid + 1;
            }
            if (lo > best)
                best = lo;
        }
    }

    /* If n has a remaining prime factor > sqrt(n) */
    if (nn > 1) {
        idx_t p = nn;
        idx_t a_exp = 1; /* since nn is prime now */

        seq_t need = (seq_t)n * (seq_t)a_exp;          /* required exponent */
        seq_t lo = 1;
        seq_t hi = need * (seq_t)p;                    /* upper bound */

        while (lo < hi) {
            seq_t mid = (lo + hi) / 2;
            if (legendre(mid, (seq_t)p) >= need)
                hi = mid;
            else
                lo = mid + 1;
        }
        if (lo > best)
            best = lo;
    }

    return best;
}
