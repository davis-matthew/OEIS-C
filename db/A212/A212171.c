/*
 * A212171  Prime signature of n (nonincreasing version): row n of table lists
 *          positive exponents in canonical prime factorization of n, in
 *          nonincreasing order.
 * https://oeis.org/A212171
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8441, u16 n <= 8441, u32 n <= 8441, u64 n <= 8441,
 *            u128 n <= 8441, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime signature of n (nonincreasing): a(n) is the n‑th element of the
   concatenated list of nonincreasing exponents from the prime factorizations
   of all integers ≥ 2.  a(2) is the first term.  Indices below 2 are
   undefined (return 0). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* Persistent state ----------------------------------------------------- */
static idx_t   *_spf        = NULL;   /* array of smallest prime factors   */
static idx_t    _spf_limit  = 0;      /* size of allocated _spf array      */
static idx_t    _next_num   = 2;      /* next integer to process           */
static uint64_t _cum_total  = 0;      /* total number of exponents processed */

/* Ensure SPF array covers at least LIMIT (inclusive). */
static void _ensure_spf(idx_t LIMIT)
{
    if (LIMIT <= _spf_limit) return;
    if (_spf) free(_spf);
    build_spf(LIMIT, &_spf);
    _spf_limit = LIMIT;
}

/* Factorize x using the SPF table, storing each exponent in exp_buf.
   exp_len is set to the number of exponents returned.               */
static void _factor_exponents(idx_t x, int *exp_len, int exp_buf[])
{
    int l = 0;
    while (x > 1) {
        idx_t p = _spf[x];
        int e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        exp_buf[l++] = e;
    }
    /* sort exponents in nonincreasing order */
    for (int i = 1; i < l; ++i) {
        int key = exp_buf[i];
        int j   = i - 1;
        while (j >= 0 && exp_buf[j] < key) {
            exp_buf[j + 1] = exp_buf[j];
            --j;
        }
        exp_buf[j + 1] = key;
    }
    *exp_len = l;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                 /* undefined below offset */

    /* target index in the flattened list: a(2) → 0, a(3) → 1, ... */
    uint64_t target = n - 2;                     /* n ≥ 2 ensures non‑negative */

    /* Process rows until the target lies within the current row. */
    for (;;) {
        /* Ensure SPF table can factor _next_num. */
        if (_next_num > _spf_limit) {
            idx_t new_limit = _spf_limit ? _spf_limit * 2 : 2;
            if (new_limit < _next_num) new_limit = _next_num;
            _ensure_spf(new_limit);
        }

        /* Compute exponents for _next_num. */
        int exps[64];
        int exp_len;
        _factor_exponents(_next_num, &exp_len, exps);

        /* If the target index falls in this row, return the element. */
        if (_cum_total + (uint64_t)exp_len > target) {
            uint64_t pos = target - _cum_total;   /* 0‑based position in row */
            return (seq_t)exps[(int)pos];
        }

        /* Move on to the next integer. */
        _cum_total += (uint64_t)exp_len;
        ++_next_num;
    }
}
