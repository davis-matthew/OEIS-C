/*
 * A118703  Zero-free primes with digit sum equal to 7.
 * https://oeis.org/A118703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Enumerate all zero‑free decimal numbers whose digits sum to 7.
   There are at most 64 such numbers, so we generate them, test each
   for primality, and store the primes in ascending order.  The function
   a(n) simply returns the (n‑1)‑th element of that array; if n exceeds
   the number of terms (29) it returns 0. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t  _candidates[64];
static size_t _cand_cnt = 0;

static seq_t  _primes[29];
static size_t _prime_cnt = 0;
static bool   _initialized = false;

/* Recursive generation of numbers with specified length, remaining sum,
   and current value. */
static void _dfs(int pos, int len, int rem, seq_t cur)
{
    if (pos == len) {
        if (rem == 0)
            _candidates[_cand_cnt++] = cur;
        return;
    }
    int max_digit = rem > 9 ? 9 : rem;
    for (int d = 1; d <= max_digit; ++d) {
        _dfs(pos + 1, len, rem - d, cur * 10 + d);
    }
}

/* Comparator for qsort on seq_t values */
static int _cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* Build the list of primes once. */
static void _build(void)
{
    /* Generate all zero‑free numbers with digit sum 7. */
    for (int len = 1; len <= 7; ++len)
        _dfs(0, len, 7, 0);

    /* Sort the candidates */
    qsort(_candidates, _cand_cnt, sizeof(seq_t), _cmp_seq);

    /* Keep only the primes. */
    for (size_t i = 0; i < _cand_cnt; ++i) {
        if (is_prime_big(_candidates[i])) {
            _primes[_prime_cnt++] = _candidates[i];
        }
    }
    _initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, return 0 for n==0 */
    if (!_initialized) _build();
    if (n - 1 < _prime_cnt) {
        return _primes[n - 1];
    }
    return 0;   /* undefined beyond the 29 known terms */
}
