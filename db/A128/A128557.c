/*
 * A128557  a(n) = the largest positive integer k such that (n!/k!) has at
 *          least n divisors.
 * https://oeis.org/A128557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A128557: a(n) = largest k such that d(n!/k!) ≥ n,
   where d(m) is the number of positive divisors of m.  */

#include <stddef.h>
#include <stdbool.h>

/*--- prime sieve ----------------------------------------------*/
#define MAX_PRIME 100000

static int s_primes[MAX_PRIME + 1];
static size_t s_prime_cnt = 0;
static bool s_primes_inited = false;

static void init_primes(void)
{
    if (s_primes_inited) return;
    static char is_comp[MAX_PRIME + 1];
    for (int i = 0; i <= MAX_PRIME; ++i) is_comp[i] = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            s_primes[s_prime_cnt++] = i;
            if ((long long)i * i <= MAX_PRIME) {
                for (int j = i * i; j <= MAX_PRIME; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    s_primes_inited = true;
}

/*--- Legendre exponent of prime p in n! -----------------------*/
static inline idx_t exp_in_fact(idx_t n, int p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/*--- check whether divisor count of (n!/k!) is at least n -------*/
static bool enough_divisors(idx_t n, idx_t k)
{
    seq_t prod = 1;
    for (size_t i = 0; i < s_prime_cnt; ++i) {
        int p = s_primes[i];
        if (p > n) break;
        idx_t e = exp_in_fact(n, p) - exp_in_fact(k, p);
        if (prod > n) return true;               /* already sufficient */
        seq_t inc = (seq_t)(e + 1);
        /* avoid overflow; if prod * inc > n, we know it's enough */
        if (prod > 0 && inc > 0 && prod > (seq_t)n / inc) return true;
        prod *= inc;
    }
    return prod >= (seq_t)n;
}

/*--- sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    init_primes();

    idx_t low = 1, high = n, ans = 1;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (enough_divisors(n, mid)) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* guard against underflow */
            high = mid - 1;
        }
    }
    return (seq_t)ans;
}
