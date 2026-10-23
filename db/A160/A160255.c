/*
 * A160255  The sum of all the entries in an n X n Cayley table for
 *          multiplication in Z_n.
 * https://oeis.org/A160255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3/2 - (n/2)*∑_{i=1..n} gcd(n,i)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* factorize n into primes[] and exps[], store count in *cnt   */
static void factorize_n(idx_t n, idx_t *primes, idx_t *exps, int *cnt)
{
    int c = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (tmp > 1)
    {
        primes[c] = tmp;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* recursively enumerate all divisors of n, accumulating the sum
 * of d * φ(n/d) in *sum */
static void rec_div(idx_t pos, seq_t cur_val, idx_t *exp_div,
                    idx_t *primes, idx_t *exps, int prcnt,
                    idx_t n, seq_t *sum)
{
    if (pos == prcnt)
    {
        /* current divisor d = cur_val (fits in idx_t) */
        idx_t d_int = (idx_t)cur_val;
        idx_t m = n / d_int;          /* m = n/d */

        /* compute φ(m) using the prime factorization of n */
        seq_t phi = 1;
        for (int i = 0; i < prcnt; ++i)
        {
            idx_t k = exps[i] - exp_div[i];
            if (k == 0) continue;

            /* part = p^(k-1) */
            seq_t part = 1;
            idx_t p = primes[i];
            for (idx_t j = 1; j < k; ++j)
                part *= (seq_t)p;

            phi *= (seq_t)(p - 1) * part;
        }

        *sum += cur_val * phi;
        return;
    }

    idx_t p = primes[pos];
    seq_t val = cur_val;
    for (idx_t e = 0; e <= exps[pos]; ++e)
    {
        exp_div[pos] = e;
        rec_div(pos + 1, val, exp_div, primes, exps, prcnt, n, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety, not used */

    /* convert n to seq_t once */
    seq_t n_seq = (seq_t)n;

    /* compute n^3 */
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;

    /* factorize n */
    idx_t primes[64];
    idx_t exps[64];
    int prcnt;
    factorize_n(n, primes, exps, &prcnt);

    /* recursive generation of divisors */
    idx_t exp_div[64] = {0};
    seq_t sum_gcd = 0;
    rec_div(0, (seq_t)1, exp_div, primes, exps, prcnt, n, &sum_gcd);

    /* a(n) = n^3/2 - (n/2)*sum_gcd */
    seq_t part1 = n3 / (seq_t)2;
    seq_t part2 = (n_seq * sum_gcd) / (seq_t)2;
    seq_t a_val = part1 - part2;
    return a_val;
}
