/*
 * A156779  a(n) = Sum_{d|n} sp(d), where sp(d) = A034387(d) = sum of primes
 *          <= d.
 * https://oeis.org/A156779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sp(d), where sp(d) = sum of primes ≤ d
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Internal static data for the cached prime list   */
static idx_t *primes = NULL;   /* primes[0..primes_cnt-1]  */
static size_t primes_cnt = 0;
static idx_t primes_limit = 0;

/*  Ensure that all primes up to “limit” are stored in the global array. */
static void _ensure_primes(idx_t limit)
{
    if (limit <= primes_limit)
        return;          /* already sufficient */

    /* free previously allocated list, if any */
    if (primes) free(primes);

    /* Simple sieve of Eratosthenes:  limit is small (≤ 10⁵ in practice ).  */
    idx_t sz = limit + 1;
    char *is_comp = (char*)calloc(sz, 1);
    if (!is_comp) exit(1);

    primes = (idx_t*)malloc(sz * sizeof(idx_t));
    if (!primes) exit(1);

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[cnt++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    primes_cnt   = cnt;
    primes_limit = limit;
    free(is_comp);
}

/*  Return a(n) = Σ_{d|n} sp(d), with sp(d) = Σ_{p prime, p ≤ d} p  */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Build prime list up to n so sp(d) can be evaluated. */
    _ensure_primes(n);

    /* Factor n and generate all its divisors. */
    idx_t tmp = n;
    idx_t divs[1024];
    size_t divs_size = 1;
    divs[0] = 1;

    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        idx_t p = primes[pi];
        if (tmp % p == 0) {
            size_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            size_t cur_sz = divs_size;
            idx_t mult = 1;
            for (size_t k = 1; k <= e; ++k) {
                mult *= p;
                for (size_t j = 0; j < cur_sz; ++j)
                    divs[divs_size++] = divs[j] * mult;
            }
        }
    }
    if (tmp > 1) {
        idx_t p = tmp;
        size_t cur_sz = divs_size;
        for (size_t j = 0; j < cur_sz; ++j)
            divs[divs_size++] = divs[j] * p;
    }

    /* Sum sp(d) over all divisors. */
    seq_t total = 0;
    for (size_t i = 0; i < divs_size; ++i) {
        idx_t d = divs[i];
        seq_t partial = 0;
        for (size_t j = 0; j < primes_cnt && primes[j] <= d; ++j)
            partial += (seq_t)primes[j];
        total += partial;
    }
    return total;
}
