/*
 * A132896  Triangle read by rows: T(n,k)=number of prime divisors of C(n,k),
 *          counted with multiplicity (0<=k<=n).
 * https://oeis.org/A132896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ω(C(r,k)) where (r,k) corresponds to the * position n in
 *            the triangle read by rows: * r = floor((sqrt(8n+1)-1)/2) * k = n
 *            – r(r+1)/2 * ω counts prime divisors of the binomial coefficient
 *            with multiplicity
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* --- exponent of prime p in n! ------------------------------------------ */
static inline seq_t _exp_factorial(idx_t n, uint64_t p)
{
    seq_t e = 0;
    idx_t tmp = n;
    while (tmp) {
        tmp /= p;
        e += tmp;
    }
    return e;
}

/* --- cached list of primes up to the largest needed limit-------------- */
static uint64_t *s_prime_list   = NULL;
static size_t    s_prime_count  = 0;
static idx_t    s_prime_limit   = 0;

static void _ensure_primes_upto(idx_t limit)
{
    if (limit <= s_prime_limit)
        return;
    if (s_prime_list)
        free(s_prime_list);
    /* build_primes_upto allocates an array of primes ≤ limit              */
    build_primes_upto(limit, &s_prime_list, &s_prime_count);
    s_prime_limit = limit;
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* determine r and k such that n = r(r+1)/2 + k, 0 ≤ k ≤ r           */
    idx_t low = 0, high = n + 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1u) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t k = n - ((idx_t)r * ((idx_t)r + 1u)) / 2u;

    if (r == 0)
        return 0;                     /* C(0,0)=1 has no prime factors */

    /* primes up to r are sufficient                                       */
    _ensure_primes_upto(r);

    seq_t sum = 0;
    for (size_t i = 0; i < s_prime_count && s_prime_list[i] <= (uint64_t)r; ++i) {
        uint64_t p = s_prime_list[i];
        seq_t e_r  = _exp_factorial(r, p);
        seq_t e_k  = _exp_factorial(k, p);
        seq_t e_rk = _exp_factorial(r - k, p);
        sum += e_r - e_k - e_rk;
    }
    return sum;
}
