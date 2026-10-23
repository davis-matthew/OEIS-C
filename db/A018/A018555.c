/*
 * A018555  Divisors of 621.
 * https://oeis.org/A018555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest divisor of 621. 621 = 3³ * 23¹. The
 *            function enumerates all divisors from this factorisation, sorts
 *            them ascending, and returns the element at index n (1‑based)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static const uint64_t _factor_primes[] = {3ULL, 23ULL};
static const uint64_t _factor_exps[]  = {3ULL, 1ULL};
static const size_t _n_factors = 2;

static void _gen_divs(size_t idx,
                      const uint64_t *pr,
                      const uint64_t *ex,
                      size_t nf,
                      seq_t cur,
                      seq_t *arr,
                      size_t *len)
{
    if (idx == nf) {
        arr[(*len)++] = cur;
        return;
    }
    const uint64_t p = pr[idx];
    const uint64_t e = ex[idx];
    seq_t v = cur;
    for (uint64_t k = 0; k <= e; ++k) {
        _gen_divs(idx + 1, pr, ex, nf, v, arr, len);
        v = v * (seq_t)p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t arr[16];
    size_t len = 0;
    _gen_divs(0, _factor_primes, _factor_exps, _n_factors, (seq_t)1, arr, &len);

    /* insertion sort – arr has at most 8 elements */
    for (size_t i = 1; i < len; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j-1] > key) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }

    if ((size_t)n > len) return (seq_t)0;
    return arr[(size_t)n - 1];
}
