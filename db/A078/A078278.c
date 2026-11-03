/*
 * A078278  a(n) is the number from which if odd-positioned digits are deleted
 *          then one gets the concatenation of first n prime numbers, while if
 *          even-positioned digits are deleted then one gets reverse
 *          concatenation of first n prime numbers. Position of least
 *          significant digit is considered to be 1.
 * https://oeis.org/A078278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is obtained by interleaving the digits of the forward
 * concatenation of the first n primes and the digit‑concatenation of
 * the same primes in reverse order.  Removing odd‑positioned digits
 * (from the least significant side) yields the forward concatenation,
 * removing even‑positioned digits yields the reverse concatenation.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Append digits of an unsigned integer in normal (MSD→LSD) order
 * to a dynamic array of uint8_t (digits 0..9). */
static void append_digits_left(idx_t num, uint8_t **arr, size_t *len, size_t *cap)
{
    uint8_t tmp[32];
    size_t t = 0;
    do {
        tmp[t++] = (uint8_t)(num % 10);
        num /= 10;
    } while (num);
    for (size_t i = 0; i < t; ++i) {
        uint8_t d = tmp[t - 1 - i];
        if (*len >= *cap) {
            size_t new_cap = *cap ? *cap * 2 : 32;
            uint8_t *tmp_arr = (uint8_t *)realloc(*arr, new_cap * sizeof(uint8_t));
            *arr = tmp_arr;
            *cap = new_cap;
        }
        (*arr)[(*len)++] = d;
    }
}

/*---------------------------------------------------------------*/
/* Prime generation – lazily computed first n primes             */
static idx_t *prime_list = NULL;
static size_t  prime_count = 0;
static size_t  prime_cap   = 0;

static void ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need) return;

    idx_t candidate = (prime_count == 0) ? 2 : prime_list[prime_count - 1] + 1;
    while (prime_count < (size_t)need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_count >= prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 32;
                idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(idx_t));
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/*---------------------------------------------------------------*/

/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* out of range; not used */

    ensure_primes(n);

    /* Build forward concatenation digits */
    uint8_t *fwd = NULL;
    size_t fwd_len = 0, fwd_cap = 0;
    for (idx_t i = 0; i < n; ++i)
        append_digits_left(prime_list[i], &fwd, &fwd_len, &fwd_cap);

    /* Build reverse concatenation digits (primes in reverse order) */
    uint8_t *rev = NULL;
    size_t rev_len = 0, rev_cap = 0;
    for (idx_t i = n; i-- > 0; )
        append_digits_left(prime_list[i], &rev, &rev_len, &rev_cap);

    /* fwd_len == rev_len */
    size_t L = fwd_len;
    seq_t result = 0;
    for (size_t j = 0; j < L; ++j) {
        result = result * 10 + (seq_t)fwd[j];
        result = result * 10 + (seq_t)rev[j];
    }

    free(fwd);
    free(rev);
    return result;
}
