/*
 * A238950  Number of arcs from even to odd level vertices in divisor lattice
 *          D(n).
 * https://oeis.org/A238950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{d|n} A001221(d)*A066829(d)
 * The implementation counts, for each divisor `d` of `n`, the
 * number of arcs from `d` to a higher divisor in the divisor lattice
 * whose level (sum of prime exponents) is odd.  That number equals
 * (#distinct primes dividing n) minus (#primes already at their
 * maximal exponent in `d`).  We simply enumerate all divisors by
 * back‑tracking over the prime exponents of `n` and sum the counts
 * for even‑level divisors.
 *
 * Complexity:  O(τ(n) * ω(n))  (τ(n)=number of divisors, ω(n)=number of distinct primes),
 * which is very small for n≤10 000.
 */
#include <stdint.h>
#include <stddef.h>

/* --- helper: factorisation ------------------------------------------------ */
static void factor_uint64(idx_t n, idx_t *pr, idx_t *ex, size_t *cnt)
{
    size_t k = 0;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[k] = p;
            ex[k] = e;
            ++k;
        }
    }
    if (tmp > 1) {          /* residual prime */
        pr[k] = tmp;
        ex[k] = 1;
        ++k;
    }
    *cnt = k;
}

/* --- helper: depth‑first enumeration of divisors -------------------------- */
static void dfs(idx_t idx, idx_t k, unsigned parity, unsigned maxcnt,
                idx_t *exponents, seq_t *sum)
{
    if (idx == k) {
        if (parity == 0) {                     /* even level only */
            *sum += (seq_t)(k - maxcnt);        /* arcs out of this divisor */
        }
        return;
    }
    for (idx_t t = 0; t <= exponents[idx]; ++t) {
        unsigned new_parity = parity ^ (t & 1U);
        unsigned new_maxcnt = maxcnt + (t == exponents[idx]);
        dfs(idx + 1U, k, new_parity, new_maxcnt, exponents, sum);
    }
}

/* --- public sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out‑of‑range safety */

    /* factorisation of n */
    idx_t primes[64];
    idx_t exponents[64];
    size_t k;
    factor_uint64(n, primes, exponents, &k);

    /* enumerate divisors and accumulate arcs from even levels */
    seq_t sum = 0;
    dfs(0, k, 0U, 0U, exponents, &sum);
    return sum;
}
