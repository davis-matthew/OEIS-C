/*
 * A066919  a(n) = least number of applications of f to n to reach 1, where f
 *          is defined by f(n) = phi(n) if n is even; = sigma(n) if n is odd.
 * https://oeis.org/A066919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A066919
 *  a(n) = smallest k ≥ 0 such that f⁽ᵏ⁾(n)=1, where
 *  f(x) = φ(x) if x is even, σ(x) if x is odd.
 *  This implementation uses trial‑division factorisation.
 */

#include <stdbool.h>
#include <stddef.h>

#define PRIME_MAX 1000000
/* prime list (≈78498 primes up to 1 000 000) */
static uint8_t _is_comp[PRIME_MAX + 1];
static uint64_t _prime_tbl[78500];
static size_t _prime_cnt;

/* initialise prime table once */
static void _init_primes(void) {
    if (_prime_cnt) return;
    for (int i = 2; i * i <= PRIME_MAX; ++i) {
        if (!_is_comp[i]) {
            for (int j = i * i; j <= PRIME_MAX; j += i)
                _is_comp[j] = 1;
        }
    }
    for (int i = 2; i <= PRIME_MAX; ++i)
        if (!_is_comp[i]) _prime_tbl[_prime_cnt++] = (uint64_t)i;
}

/* φ(n)  (Euler's totient) */
static seq_t _phi(seq_t n) {
    seq_t result = n;
    seq_t m = n;
    for (size_t i = 0; i < _prime_cnt && _prime_tbl[i] * _prime_tbl[i] <= m; ++i) {
        uint64_t p = _prime_tbl[i];
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            result -= result / p;
        }
    }
    if (m > 1) result -= result / m;
    return result;
}

/* σ(n)  (sum of divisors) */
static seq_t _sigma(seq_t n) {
    seq_t res = 1;
    seq_t m = n;
    for (size_t i = 0; i < _prime_cnt && _prime_tbl[i] * _prime_tbl[i] <= m; ++i) {
        uint64_t p = _prime_tbl[i];
        if (m % p == 0) {
            seq_t term = 1;
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            res *= term;
        }
    }
    if (m > 1) {
        /* remaining prime factor */
        res *= ((seq_t)1 + m);
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset below 1 is irrelevant */

    _init_primes();

    seq_t cur = (seq_t)n;
    seq_t steps = 0;
    while (cur != (seq_t)1) {
        ++steps;
        if ((cur & (seq_t)1) == 0) {          /* even */
            cur = _phi(cur);
        } else {                              /* odd */
            cur = _sigma(cur);
        }
    }
    return steps;
}
