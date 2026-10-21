/*
 * A092612  Product of first n primes that end in 7.
 * https://oeis.org/A092612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of first n primes that end in 7
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 18,
 *            big any n
 * Helpers:   idx_alloc, idx_realloc, is_prime_simple, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/idx_alloc.h"
#include "../utils/idx_realloc.h"
#include "../utils/is_prime_simple.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Cache structure */
struct seq_data {
    seq_t* products;
    size_t products_len;
    size_t products_cap;
    idx_t* primes7;
    size_t primes7_len;
    size_t primes7_cap;
    idx_t next_candidate;
};

/* Singleton cache instance initialized to zero */
static struct seq_data _seq_data = {0};

/* Ensure all products up to n have been generated */
static void seq_ensure_upto(idx_t n) {
    if (_seq_data.products_cap == 0) {
        _seq_data.products_cap = 16;
        _seq_data.products = seq_alloc(_seq_data.products_cap);
    }
    if (_seq_data.primes7_cap == 0) {
        _seq_data.primes7_cap = 16;
        _seq_data.primes7 = idx_alloc(_seq_data.primes7_cap);
    }
    if (_seq_data.next_candidate == 0)          /* start with 7 */
        _seq_data.next_candidate = 7;

    while (_seq_data.products_len < n) {
        idx_t cand = _seq_data.next_candidate;

        /* find the next prime ending in 7 */
        while (!is_prime_simple(cand))
            cand += 10;

        /* store the prime */
        if (_seq_data.primes7_len == _seq_data.primes7_cap) {
            _seq_data.primes7_cap *= 2;
            _seq_data.primes7 = idx_realloc(_seq_data.primes7,
                                            _seq_data.primes7_len,
                                            _seq_data.primes7_cap);
        }
        _seq_data.primes7[_seq_data.primes7_len++] = cand;

        /* compute cumulative product */
        seq_t prod;
        if (_seq_data.products_len == 0)
            prod = (seq_t)cand;
        else
            prod = _seq_data.products[_seq_data.products_len - 1] * (seq_t)cand;

        if (_seq_data.products_len == _seq_data.products_cap) {
            _seq_data.products_cap *= 2;
            _seq_data.products = seq_realloc(_seq_data.products,
                                             _seq_data.products_len,
                                             _seq_data.products_cap);
        }
        _seq_data.products[_seq_data.products_len++] = prod;

        /* advance to the next candidate */
        _seq_data.next_candidate = cand + 10;
    }
}

/* Public function required by the exercise */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; value for 0 is undefined */
    seq_ensure_upto(n);
    return _seq_data.products[n - 1];
}
