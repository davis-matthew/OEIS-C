/*
 * A214750  Least m > 0 such that n - m divides n^2 + m^2.
 * https://oeis.org/A214750
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - max{ d < n : d divides 2 * n^2 }
 * Range:     u8 n <= 262, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least m > 0 such that n - m divides n^2 + m^2. */

#include <stdbool.h>
#include <stddef.h>

/* Factorise a number into primes and exponents.
   primes[64] and exps[64] must be large enough to hold the result. */
static void factorize_n(idx_t n, idx_t *primes, int *exps, size_t *cnt)
{
    size_t c = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; p = (p == 2) ? 3 : p + 2) {
        if (tmp % p == 0) {
            primes[c] = p;
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            exps[c] = e;
            ++c;
        }
    }
    if (tmp > 1) {          /* remaining prime */
        primes[c] = tmp;
        exps[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursive enumeration of all divisors of 2*n^2 using its prime factorisation.
   best holds the largest divisor found that is < n. */
static void dfs_div(const idx_t *primes, const int *exps, size_t cnt,
                    size_t pos, idx_t cur, idx_t n, idx_t *best)
{
    if (cur < n && cur > *best)
        *best = cur;

    if (pos == cnt)
        return;

    idx_t p = primes[pos];
    idx_t val = cur;
    for (int e = 0; e <= exps[pos]; ++e) {
        if (val <= n)
            dfs_div(primes, exps, cnt, pos + 1, val, n, best);

        if (e == exps[pos])
            break;
        /* Multiply by p for next exponent, but stop if it would exceed n
           to keep search space small; any higher multiples will also exceed n. */
        if (val > n / p)
            break;
        val *= p;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                 /* behaviour below offset is irrelevant */

    /* Factorise n. */
    idx_t primes[64];
    int exps[64];
    size_t cnt;
    factorize_n(n, primes, exps, &cnt);

    /* Build factorisation of 2 * n^2. */
    idx_t primes2[64];
    int exps2[64];
    size_t total = 0;
    bool has_two = false;

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            has_two = true;
            primes2[total] = 2;
            exps2[total] = 2 * exps[i] + 1;   /* extra factor of 2 */
        } else {
            primes2[total] = primes[i];
            exps2[total] = 2 * exps[i];
        }
        ++total;
    }
    if (!has_two) {                     /* n is odd: add factor 2^1 */
        primes2[total] = 2;
        exps2[total] = 1;
        ++total;
    }

    /* Find the largest divisor of 2*n^2 that is < n. */
    idx_t best = 1;                     /* 1 always divides any number */
    dfs_div(primes2, exps2, total, 0, 1, n, &best);

    /* Result is n - best. */
    return (seq_t)(n - best);
}
