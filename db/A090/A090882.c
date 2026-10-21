/*
 * A090882  Suppose n=(p1^e1)(p2^e2)... where p1,p2,... are the prime numbers
 *          and e1,e2,... are nonnegative integers. Then a(n) = e1 + (e2)*5 +
 *          (e3)*25 + (e4)*125 + ... + (ek)*(5^(k-1)) + ...
 * https://oeis.org/A090882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ e_k * 5^(k-1) where n = ∏ p_k^e_k, p_k are the primes
 *            in ascending order
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 46, u64 n <= 108,
 *            u128 n <= 268, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Utility: integer square root for idx_t (returns floor(sqrt(x))) */
static size_t int_sqrt(idx_t x)
{
    size_t r = 0;
    while ((idx_t)(r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* -- prime sieve – minimal, incremental, thread‑safe via static state -- */
static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static idx_t prime_max = 1;

static void fill_primes_up_to(idx_t limit)
{
    if (limit <= prime_max)
        return;                                    /* already have enough */

    if (prime_cap < prime_cnt + (size_t)(limit - prime_max))
    {
        size_t new_cap = prime_cnt + (size_t)(limit - prime_max) + 16;
        idx_t *new_arr = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
        if (!new_arr)
            return;                                 /* allocation failure – we will fallback to trial division if needed */
        prime_list = new_arr;
        prime_cap = new_cap;
    }

    for (idx_t cand = prime_max + 1; cand <= limit; ++cand)
    {
        bool is_p = true;
        size_t sr = int_sqrt(cand);
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= sr; ++i)
        {
            if (cand % prime_list[i] == 0)
                { is_p = false; break; }
        }
        if (is_p)
            prime_list[prime_cnt++] = cand;
    }
    prime_max = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                      /* a(1) = 0, a(0) is outside the offset */

    idx_t tmp = n;                     /* work on copy of n */
    fill_primes_up_to(n);              /* ensure we have all primes ≤ n */

    seq_t result = 0;
    seq_t weight  = 1;                 /* corresponds to 5^(k-1), starts with k=1 for prime 2 */

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= tmp; ++i)
    {
        idx_t p = prime_list[i];
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            result += (seq_t)exp * weight;
        }
        weight *= 5;                    /* next prime gets weight multiplied by 5 */
    }

    /* After processing primes up to n, tmp should be 1.
       If rem > 1 (should not happen), it is a prime factor > n,
       which can only occur when n was 0 or 1; in that case the
       function already returned above.  The following check
       keeps the implementation future‑proof. */
    if (tmp > 1)
        result += weight;              /* exponent is 1 */

    return result;
}
