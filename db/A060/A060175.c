/*
 * A060175  Square array A(n,k) = exponent of the largest power of k-th prime
 *          which divides n, read by falling antidiagonals.
 * https://oeis.org/A060175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, changed
 * Formula:   a(n) = v_{p_k}(i) where k and i are obtained from the
 *            antidiagonal mapping: find t such that T_{t-1} < n ≤ T_t (T_m =
 *            m(m+1)/2), let i = n - T_{t-1}, let k = t + 1 - i. Then a(n) is
 *            the exponent of the k‑th prime p_k in the factorisation of i
 * Range:     u8 n <= 9117, u16 n <= 9117, u32 n <= 9117, u64 n <= 9117,
 *            u128 n <= 9117, big any n
 * Helpers:   _prime_exp_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_prime_exp_u64.h"

/* static state for prime generation */
static unsigned _primes[2000];
static int _prime_cnt = 0;
static int _prime_limit = 20000;
static unsigned char _is_composite[20001] = {0};

static void _init_primes(void)
{
    int i, j;
    for (i = 2; i <= _prime_limit && _prime_cnt < 2000; ++i) {
        if (!_is_composite[i]) {
            _primes[_prime_cnt++] = (unsigned)i;
            if ((long long)i * i <= _prime_limit) {
                for (j = i * i; j <= _prime_limit; j += i) {
                    _is_composite[j] = 1;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    static int _initialized = 0;
    if (!_initialized) {
        _init_primes();
        _initialized = 1;
    }

    /* find t such that T_{t-1} < n <= T_t */
    idx_t t = 1;
    while (n > t * (t + 1) / 2) {
        ++t;
    }

    idx_t t_prev = (t - 1) * t / 2;
    idx_t i = n - t_prev;           /* row index (1‑based) */
    idx_t k = t + 1 - i;            /* column index (1‑based) */
    if (k > (idx_t)_prime_cnt) {
        /* unlikely for n <= 10000, but guard */
        return (seq_t)0;
    }

    unsigned prime_k = _primes[k - 1];
    return _prime_exp_u64((unsigned long long)i, prime_k);
}
