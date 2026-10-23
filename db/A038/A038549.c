/*
 * A038549  Least number with exactly n divisors that are at most its square
 *          root.
 * https://oeis.org/A038549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(A005179(2n-1), A005179(2n))
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 78, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least integer with exactly n unordered factorizations x*y
   (equivalently, least number with 2n or 2n-1 divisors). */

#include <stdint.h>
#include <stdbool.h>

/* Maximum divisor count needed.  For n up to 10 000, we need up to 20 000. */
enum { MAX_D = 20000 };

/* First 16 primes are enough to generate all divisor counts up to MAX_D. */
static const uint64_t _primes[16] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL,
    13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL,
    53ULL
};
static const idx_t _prime_cnt = 16;

/* Array holding the minimal number for each divisor count 1..MAX_D.
   0 indicates “not yet filled” (for divisor count 0 we never index). */
static seq_t _best[MAX_D + 1];
static bool _initialized = false;

/* Recursive search to populate _best.  Non‑increasing exponents
   are ensured by passing prev_e, the upper bound on the next exponent. */
static void _dfs(idx_t p_idx, idx_t prev_e, seq_t cur_num, idx_t cur_dcnt)
{
    if (p_idx >= _prime_cnt)
        return;

    seq_t prime = (seq_t)_primes[p_idx];
    seq_t pow = 1;                         /* prime^e, start with exponent 0 */

    for (idx_t e = 1; e <= prev_e; ++e) {
        /* Stop if the resulting divisor count would exceed MAX_D. */
        if (cur_dcnt * (e + 1) > MAX_D)
            break;

        /* Compute prime^e; detect unsigned overflow by comparing
           the new value to the old ones.  For big‑integer types, this
           comparison is always false. */
        seq_t new_pow = pow * prime;
        if (new_pow < pow || new_pow < prime)
            break;                         /* overflow or wrap‑around */
        pow = new_pow;

        idx_t new_dcnt = cur_dcnt * (e + 1);

        /* New candidate number.  Detect overflow in the same manner. */
        seq_t new_num = cur_num * pow;
        if (new_num < cur_num || new_num < pow)
            break;                         /* overflow */

        /* Record minimal value for this divisor count. */
        if (_best[new_dcnt] == 0 || new_num < _best[new_dcnt])
            _best[new_dcnt] = new_num;

        /* Recurse for the next prime, allowing exponent <= e. */
        _dfs(p_idx + 1, e, new_num, new_dcnt);
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Lazy initialization – compute once. */
    if (!_initialized) {
        _best[1] = (seq_t)1;              /* 1 has exactly 1 divisor */
        _dfs(0, MAX_D, (seq_t)1, 1);
        _initialized = true;
    }

    idx_t d1 = 2 * n - 1;
    idx_t d2 = 2 * n;

    seq_t val1 = _best[d1];
    seq_t val2 = _best[d2];

    /* One of the two values is guaranteed to be non‑zero. */
    if (val1 == 0)
        return val2;
    if (val2 == 0)
        return val1;
    return (val1 < val2) ? val1 : val2;
}
