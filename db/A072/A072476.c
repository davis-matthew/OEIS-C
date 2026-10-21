/*
 * A072476  Difference between the sum of first n prime numbers and the sum of
 *          first n composite numbers.
 * https://oeis.org/A072476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{i=1}^{n} prime_i - sum_{i=1}^{n} composite_i
 * Range:     i8 n <= 15, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t      _max_n          = 0;
static uint32_t   *_primes        = NULL;
static size_t     _primes_cap     = 0;
static size_t     _primes_cnt     = 0;
static seq_t     *_prime_sums    = NULL;
static size_t     _prime_sums_cap = 0;
static uint32_t   *_comps        = NULL;
static size_t     _comps_cap      = 0;
static size_t     _comps_cnt      = 0;
static seq_t     *_comp_sums     = NULL;
static size_t     _comp_sums_cap  = 0;
static idx_t      _cur_num        = 2;

static bool _is_prime(uint32_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < _primes_cnt; ++i) {
        uint32_t p = _primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static void _grow_primes(size_t needed)
{
    if (_primes_cap >= needed) return;
    size_t new_cap = _primes_cap ? _primes_cap * 2 : 1024;
    while (new_cap < needed) new_cap *= 2;
    _primes = (uint32_t*)realloc(_primes, new_cap * sizeof(uint32_t));
    _primes_cap = new_cap;
    _prime_sums = (seq_t*)realloc(_prime_sums, new_cap * sizeof(seq_t));
    _prime_sums_cap = new_cap;
}

static void _grow_comps(size_t needed)
{
    if (_comps_cap >= needed) return;
    size_t new_cap = _comps_cap ? _comps_cap * 2 : 1024;
    while (new_cap < needed) new_cap *= 2;
    _comps = (uint32_t*)realloc(_comps, new_cap * sizeof(uint32_t));
    _comps_cap = new_cap;
    _comp_sums = (seq_t*)realloc(_comp_sums, new_cap * sizeof(seq_t));
    _comp_sums_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (_max_n < n) {
        _grow_primes((size_t)n + 1);
        _grow_comps((size_t)n + 1);
        while (_primes_cnt < (size_t)n || _comps_cnt < (size_t)n) {
            uint32_t cur = (uint32_t)_cur_num;
            if (_is_prime(cur)) {
                _primes[_primes_cnt] = cur;
                if (_primes_cnt == 0)
                    _prime_sums[_primes_cnt] = (seq_t)cur;
                else
                    _prime_sums[_primes_cnt] = _prime_sums[_primes_cnt - 1] + (seq_t)cur;
                ++_primes_cnt;
            } else if (cur > 1) {
                _comps[_comps_cnt] = cur;
                if (_comps_cnt == 0)
                    _comp_sums[_comps_cnt] = (seq_t)cur;
                else
                    _comp_sums[_comps_cnt] = _comp_sums[_comps_cnt - 1] + (seq_t)cur;
                ++_comps_cnt;
            }
            ++_cur_num;
        }
        _max_n = n;
    }
    seq_t sumPr = _prime_sums[(size_t)n - 1];
    seq_t sumComp = _comp_sums[(size_t)n - 1];
    return sumPr - sumComp;
}
