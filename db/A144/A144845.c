/*
 * A144845  Least number k such that all coefficients of k*B(n,x), the n-th
 *          Bernoulli polynomial, are integers.
 * https://oeis.org/A144845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all primes p ≤ n+1 for which there exists * k
 *            ≥ p–1, k ≡ 0 (mod p–1), k ≤ n such that C(n,k) is not divisible
 *            by p. * This is equivalent to the primes appearing in the
 *            denominator of the * n‑th Bernoulli polynomial B(n,x). * The
 *            test "C(n,k) not divisible by p" is performed usin...
 * Range:     u8 n <= 11, u16 n <= 35, u32 n <= 111, u64 n <= 419,
 *            u128 n <= 1259, big any n
 * Validated: standard, pass, 68 terms through n = 9545 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: returns true iff C(n,k) is NOT divisible by prime p. */
static bool binom_not_divisible_by_p(idx_t n, idx_t k, uint32_t p)
{
    /* Apply Lucas theorem: compute base‑p digits of n and k. */
    while (k != 0) {
        uint32_t n_digit = (uint32_t)(n % p);
        uint32_t k_digit = (uint32_t)(k % p);
        if (k_digit > n_digit)    /* C(n_i,k_i)=0 ⇒ C(n,k)≡0 (mod p) */
            return false;
        n /= p;
        k /= p;
    }
    return true; /* all digit conditions satisfied → binomial not divisible */
}

seq_t a(idx_t n)
{
    if (n == 0)      /* a(0) = 1 */
        return (seq_t)1;

    idx_t limit = n + 1;

    /* ---------- sieve of Eratosthenes for primes ≤ n+1 ---------- */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime) return (seq_t)0;          /* allocation failed */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = 0;

    /* collect primes into an array */
    size_t estimated_primes = limit / 2 + 10;          /* safe upper bound */
    uint32_t *primes = (uint32_t *)malloc(estimated_primes * sizeof(uint32_t));
    if (!primes) { free(is_prime); return (seq_t)0; }

    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[primes_cnt++] = (uint32_t)i;

    /* ---------- compute a(n) by the prime–cancellation rule ---------- */
    seq_t ans = (seq_t)1;

    /* prime 2 always contributes for n ≥ 1 (C(n,n)=1 is not divisible) */
    if (n >= 1)
        ans = mul_small(ans, 2);

    /* handle other primes */
    for (size_t i = 1; i < primes_cnt; ++i)   /* start at 1 → skip the prime 2 */
    {
        uint32_t p = primes[i];
        uint32_t d = p - 1;                  /* the period for which p divides D_k */

        if (d > n)            /* D_k never contains p for k ≤ n */
            continue;

        bool keep = false;

        /* If (p-1) divides n, k = n works: C(n,n)=1 is not divisible by p. */
        if ((n % d) == 0)
            keep = true;
        else
        {
            /* check all multiples k = m*d ≤ n */
            idx_t max_k = (n / d) * d;
            for (idx_t k = d; k <= max_k; k += d)
                if (binom_not_divisible_by_p(n, k, p))
                {
                    keep = true;
                    break;
                }
        }

        if (keep)
            ans = mul_small(ans, p);
    }

    free(is_prime);
    free(primes);

    return ans;
}
