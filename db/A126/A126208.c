/*
 * A126208  Number of 6's in decimal expansion of 6^n.
 * https://oeis.org/A126208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A126208
   a(n) = number of digit '6' in the decimal expansion of 6^n
   The implementation multiplies the current power of 6 digit-wise,
   counting the number of 6’s after each step. Cached results allow O(1)
   retrieval for any previously computed n.  */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Helper structure to keep state between calls */
struct _pow6_state {
    uint8_t *digits;          /* LSB‑first decimal digits of 6^current_n */
    size_t   len;             /* number of used digits */
    size_t   cap;             /* allocated capacity for digits */
    seq_t   *cnt;             /* cached counts of digit 6; index = n */
    size_t   cnt_cap;         /* allocated capacity for cnt array */
    size_t   current_n;       /* n such that digits represent 6^n */
} _power6 = { NULL, 0, 0, NULL, 0, 0 };

/* Ensure the digits array has at least 'min_cap' bytes capacity */
static void
ensure_digits_cap(size_t min_cap)
{
    if (_power6.cap >= min_cap) return;
    size_t new_cap = _power6.cap ? _power6.cap << 1 : 64;
    while (new_cap < min_cap) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(_power6.digits, new_cap);
    if (!tmp) abort();
    _power6.digits = tmp;
    _power6.cap    = new_cap;
}

/* Ensure the cnt array has at least 'min_cap' elements */
static void
ensure_cnt_cap(size_t min_cap)
{
    if (_power6.cnt_cap >= min_cap) return;
    size_t new_cap = _power6.cnt_cap ? _power6.cnt_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(_power6.cnt, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    _power6.cnt   = tmp;
    _power6.cnt_cap = new_cap;
}

/* Multiply the current decimal number by 6 and update the cache. */
static void
multiply_by_six(void)
{
    uint16_t carry = 0;   /* product fits in 16 bits (max 9*6+5) */
    for (size_t i = 0; i < _power6.len; ++i) {
        uint16_t prod = (uint16_t)_power6.digits[i] * 6 + carry;
        _power6.digits[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        ensure_digits_cap(_power6.len + 1);
        _power6.digits[_power6.len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }

    /* Count digit 6 in the new representation */
    size_t count6 = 0;
    for (size_t i = 0; i < _power6.len; ++i)
        if (_power6.digits[i] == 6)
            ++count6;

    /* Store the count */
    ensure_cnt_cap(_power6.current_n + 2);          /* plus one for n, plus one for 0‑index */
    _power6.cnt[_power6.current_n + 1] = (seq_t)count6;
    _power6.current_n++;                          /* now represents 6^(current_n+1) */
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    /* One‑time initialisation */
    if (_power6.digits == NULL) {
        ensure_digits_cap(16);
        _power6.digits[0] = 1;
        _power6.len       = 1;
        _power6.current_n = 0;

        ensure_cnt_cap(1);
        _power6.cnt[0] = (seq_t)0;   /* a(0) = 0 */
    }

    /* If the requested index is already cached, return immediately */
    if (_power6.current_n >= (size_t)n)
        return _power6.cnt[n];

    /* Compute powers up to the requested index */
    while (_power6.current_n < (size_t)n)
        multiply_by_six();

    return _power6.cnt[n];
}
