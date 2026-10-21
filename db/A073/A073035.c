/*
 * A073035  Take A000040, omit commas: 23571113171923..., select 2-digit
 *          primes seen when scanning from left.
 * https://oeis.org/A073035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _is_two_digit_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/_is_two_digit_prime.h"

/* Implementation of OEIS sequence A073035.
 *
 * The sequence is obtained by concatenating all prime numbers
 * (2357111317…) and scanning left‑to‑right for overlapping
 * two‑digit windows.  Each such window that represents a prime
 * (leading zeros are allowed, so 03 counts as 3) is output as a
 * term.  The function implements lazy generation with memoisation,
 * so it works for any order of calls.  The maximum number of
 * terms required by the tests (≤10000) is comfortably handled by
 * the static array below.  All arithmetic is done with the
 * supplied integer types `seq_t` and `idx_t`. */

#define MAX_TERMS 20000                     /* capacity for stored terms */

/* --------------------------------------------------------------------- */
/* Sequence storage */
static size_t _seq_len = 0;                 /* number of terms stored   */
static seq_t _seq[MAX_TERMS];               /* generated terms          */

/* --------------------------------------------------------------------- */
/* Prime generation (trial division) */
static size_t _prime_cnt = 0;               /* number of primes found   */
static idx_t  _primes[MAX_TERMS];           /* list of primes           */
static idx_t  _candidate = 2;               /* next integer to test     */

/* forward declarations */
static bool   _is_prime_candidate(idx_t n);
static void   _generate_next_prime(void);
static bool   _is_two_digit_prime(int x);

/* test whether n is prime using the primes already found */
static bool _is_prime_candidate(idx_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < _prime_cnt; ++i) {
        idx_t p = _primes[i];
        if ((idx_t)p * (idx_t)p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* generate the next prime, store it in _primes */
static void _generate_next_prime(void)
{
    while (!_is_prime_candidate(_candidate))
        ++_candidate;
    _primes[_prime_cnt++] = _candidate;
    ++_candidate;
}

/* store a new term; the array is large enough for 20000 terms */
static void _add_term(seq_t val)
{
    if (_seq_len < MAX_TERMS) {
        _seq[_seq_len++] = val;
    }
}

/* keep the previous digit from the concatenated prime string */
static int _prev_digit = -1;

/* process a prime number: examine its digits left‑to‑right,
 * producing two‑digit primes when found. */
static void _process_prime(idx_t p)
{
    /* extract digits into an array (least significant first) */
    int digits[20];
    size_t len = 0;
    idx_t x = p;
    do {
        digits[len++] = (int)(x % 10);
        x /= 10;
    } while (x > 0);

    /* iterate digits from most significant to least */
    for (size_t i = len; i-- > 0; ) {
        int d = digits[i];
        if (_prev_digit >= 0) {
            int val = _prev_digit * 10 + d;
            if (_is_two_digit_prime(val)) {
                _add_term((seq_t)val);
            }
        }
        _prev_digit = d;
    }
}

/* --------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1; undefined for n=0 */

    /* generate terms until we have at least n of them */
    while (_seq_len < (size_t)n) {
        _generate_next_prime();            /* next prime number          */
        _process_prime(_primes[_prime_cnt - 1]); /* process its digits   */
    }
    return _seq[n - 1];
}
