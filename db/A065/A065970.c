/*
 * A065970  a(n) = n^2 * Sum_{primes p dividing n} (1 - 1/p^2).
 * https://oeis.org/A065970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n^2 * Σ_{p|n} (1 - 1/p^2)
 * Equivalent implementation:
 *   let k = number of distinct prime divisors of n
 *   a(n) = n^2 * k - Σ_{p|n} (n/p)^2
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    const int MAX_PRIMES = 64;          /* maximal number of distinct primes for any n in typical test range */
    idx_t primes[MAX_PRIMES];
    int cnt = 0;

    idx_t tmp = n;

    /* factor out 2 */
    if ((tmp & 1) == 0) {
        primes[cnt++] = 2;
        while ((tmp & 1) == 0) tmp >>= 1;
    }

    /* odd factors */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            primes[cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }

    /* remaining prime > sqrt(tmp) */
    if (tmp > 1) {
        primes[cnt++] = tmp;
    }

    /* compute a(n) */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t ans = n_sq * (seq_t)cnt;      /* n^2 * k */

    for (int i = 0; i < cnt; ++i) {
        idx_t p = primes[i];
        idx_t t = n / p;
        seq_t t_sq = (seq_t)t * (seq_t)t;
        ans -= t_sq;                    /* subtract (n/p)^2 for each prime divisor */
    }

    return ans;
}
