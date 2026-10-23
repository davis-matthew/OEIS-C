/*
 * A119988  Divisors of 69371610.
 * https://oeis.org/A119988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 69371610 in ascending order (OEIS A119988).
   a(n) is the nth smallest divisor of 69371610.  This implementation
   builds all 128 divisors, stores them in a static array, sorts the
   array once on the first call, and then returns the requested element.
   The generated divisors list is independent of the order of calls
   and works for any supported seq_t type. */

#include <stdlib.h>

/* prime factors of 69371610 */
static const seq_t _primes[7] = { 2, 3, 5, 7, 11, 59, 509 };

/* exponent of each prime factor (all 1) */
static const int _exps[7] = { 1, 1, 1, 1, 1, 1, 1 };

/* storage for all 128 divisors */
static seq_t _divs[128];
static size_t _div_count = 0;
static int _initialized = 0;

/* comparison function for qsort (ascending) */
static int _cmp_seq(const void *a, const void *b)
{
    const seq_t va = *(const seq_t *)a;
    const seq_t vb = *(const seq_t *)b;
    return (va > vb) - (va < vb); /* negative if va<vb, positive if va>vb */
}

/* recursive generation of divisors */
static void _gen_divs(int idx, seq_t cur, size_t *pos)
{
    if (idx == 7) {
        _divs[*pos] = cur;
        ++(*pos);
        return;
    }
    seq_t val = cur;
    for (int e = 0; e <= _exps[idx]; ++e) {
        _gen_divs(idx + 1, val, pos);
        val *= _primes[idx];
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (!_initialized) {
        /* generate all divisors */
        _div_count = 1;
        for (int i = 0; i < 7; ++i) _div_count *= (size_t)(_exps[i] + 1);  /* 2^7 = 128 */

        size_t pos = 0;
        _gen_divs(0, (seq_t)1, &pos);

        /* sort ascending */
        qsort(_divs, _div_count, sizeof(seq_t), _cmp_seq);

        _initialized = 1;
    }
    if (n == 0 || n > (idx_t)_div_count) return (seq_t)0;
    return _divs[(size_t)(n - 1)];
}
