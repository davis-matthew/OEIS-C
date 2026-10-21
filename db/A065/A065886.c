/*
 * A065886  Smallest square divisible by n!.
 * https://oeis.org/A065886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p≤n} p^{ceil_even(v_p(n!))}
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where v_p(n!) = ∑_{k≥1} ⌊n/p^k⌋   and   ceil_even(x) = x + (x&1)  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *prime_arr = NULL;   /* array of found primes   */
static size_t prime_cnt = 0;         /* number of primes stored*/
static uint64_t prime_last = 1;      /* largest prime in array */

/* extend prime array up to limit (inclusive) */
static void extend_primes(uint64_t limit)
{
    if (limit <= prime_last)
        return;

    /* start searching from prime_last+1 */
    for (uint64_t cand = prime_last + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_arr[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == 0 || prime_arr == NULL || prime_cnt * sizeof(uint64_t) > 0) {
                /* ensure capacity */
                size_t new_cap = prime_cnt + 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
                if (!tmp) {
                    /* allocation failure: abort */
                    abort();
                }
                prime_arr = tmp;
                prime_cnt = new_cap - 1; /* will be overwritten below */
            }
            prime_arr[prime_cnt++] = cand;
            prime_last = cand;
        }
    }
}

/* fast exponentiation: base^exp (exp fits in idx_t) */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1)
            res = res * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 1;   /* 0! = 1! = 1, smallest square is 1 */

    /* generate all primes up to n */
    extend_primes(n);

    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= n; ++i) {
        uint64_t p = prime_arr[i];
        /* compute exponent of p in n! */
        idx_t v = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            v += t;
        }
        /* make exponent even */
        if (v & 1)
            ++v;
        /* multiply result by p^v */
        seq_t p_pow = pow_seq_t((seq_t)p, v);
        result = result * p_pow;
    }
    return result;
}
