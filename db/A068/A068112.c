/*
 * A068112  Denominator of coefficient of (-x^2)^n in F(x)*F(-x) where F(x) =
 *          Sum_{k>=0} x^k/(k!)^3.
 * https://oeis.org/A068112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denominator((3*n)!/(n!*(2*n)!)^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   mul_pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul_pow_seq.h"

/* Squared version of the Legendre formula.
   Returns v_p(n!) for prime p. */
static inline int v_p_factorial(idx_t n, int p)
{
    int cnt = 0;
    idx_t power = p;
    while (power <= n)
    {
        cnt += n / power;
        power *= p;
    }
    return cnt;
}

/* Pre–generation of primes up to the maximum needed (3*max_n). */
#define MAX_N   30000
static int primelist[MAX_N + 1];
static int prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    static bool is_prime[MAX_N + 1];
    for (int i = 2; i <= MAX_N; ++i) is_prime[i] = true;
    for (int i = 2; i * i <= MAX_N; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= MAX_N; j += i) is_prime[j] = false;
    for (int i = 2; i <= MAX_N; ++i)
        if (is_prime[i]) primelist[prime_cnt++] = i;
    primes_inited = true;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    if (!primes_inited) init_primes();

    idx_t limit = 3 * n;
    seq_t result = 1;

    /* Iterate over primes <= limit */
    for (int i = 0; i < prime_cnt; ++i) {
        int p = primelist[i];
        if ((idx_t)p > limit) break;

        int vp_n    = v_p_factorial(n,        p);
        int vp_2n   = v_p_factorial(2 * n,    p);
        int vp_3n   = v_p_factorial(3 * n,    p);

        int exp = 3 * (vp_n + vp_2n) - vp_3n;
        if (exp > 0) {
            mul_pow_seq(&result, p, exp);
        }
    }
    return result;
}
