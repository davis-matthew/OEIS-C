/*
 * A118703  Zero-free primes with digit sum equal to 7.
 * https://oeis.org/A118703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: full, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   is_prime_u64, mem
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
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
    mem_qsort(_candidates, _cand_cnt, sizeof(seq_t), _cmp_seq);

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
