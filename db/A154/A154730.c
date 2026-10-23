/*
 * A154730  Decimal expansion of 1 minus the Copeland-Erdős constant.
 * https://oeis.org/A154730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn, base
 * Formula:   a(n) = 9 - the n-th decimal digit of the Copeland‑Erdős constant
 *            (the concatenation of primes: 2,3,5,7,11,13,17,19,23,29,31,…)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _prime_gen_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_gen_next.h"

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* ---------- Digit storage for the Copeland‑Erdős constant ------------- */
static char *_c_digits = NULL;        /* array of digits as characters */
static size_t _c_len = 0;              /* number of digits stored */
static size_t _c_cap = 0;              /* capacity of the digit array */

/* Ensure we have at least (n+1) digits stored in _c_digits */
static void _ensure_digits(idx_t n)
{
    while (_c_len <= n) {
        /* get next prime */
        uint32_t p = _prime_gen_next();

        /* convert prime to decimal digits (normal order) */
        char buf[12];   /* enough for uint32_t */
        size_t b = 0;
        uint32_t tmp = p;
        do {
            buf[b++] = (char)('0' + (tmp % 10));
            tmp /= 10;
        } while (tmp);

        /* reverse order and append to digit array */
        if (_c_len + b > _c_cap) {
            _c_cap = (_c_cap == 0) ? 64 : _c_cap * 2;
            _c_digits = (char *)realloc(_c_digits, _c_cap);
        }
        for (size_t i = 0; i < b; ++i)
            _c_digits[_c_len++] = buf[b - 1 - i];
    }
}

/* ---------- Sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    _ensure_digits(n);

    /* the n-th digit of the Copeland‑Erdős constant */
    int digit = _c_digits[n] - '0';
    return (seq_t)(9 - digit);
}
