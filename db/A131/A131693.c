/*
 * A131693  Numbers n such that S(n) = 0, where S(n) = (S(n-1) +
 *          A000040(n+1))*(-1)^n; S(0)=0, n >= 1.
 * https://oeis.org/A131693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A131693: the numbers n for which
 *   S(0) = 0,
 *   S(n) = (S(n‑1) + A000040(n+1)) * (‑1)^n          (n ≥ 1).
 *
 * The function returns the n‑th term a(n) (1‑based).  It works for
 * arbitrary integer types seq_t and idx_t, allocating prime and
 * zero‑position tables lazily and recomputing the recurrence only
 * as far as needed.  The code is fully conformant with C11/C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Simple prime infrastructure ------------------------------------------------ */
static uint64_t *prime_list    = NULL;   /* primes 2,3,5,7,... */
static size_t   prime_len      = 0;
static size_t   prime_cap      = 0;
static uint64_t last_prime_candidate = 2;  /* next candidate to test */

/* Ensure prime_list can hold n primes; grow by factor 2. */
static void _ensure_prime_cap(size_t needed) {
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Test whether n is prime using trial division by primes < sqrt(n). */
static bool _primality_test(uint64_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Return the k‑th prime (1‑based) and grow the prime list if needed. */
static uint64_t _get_prime(idx_t k) {
    _ensure_prime_cap((size_t)k);
    while (prime_len < (size_t)k) {
        if (_primality_test(last_prime_candidate)) {
            prime_list[prime_len++] = last_prime_candidate;
        }
        if (last_prime_candidate == 2)
            last_prime_candidate = 3;
        else
            last_prime_candidate += 2;
    }
    return prime_list[k - 1];
}

/* ------------------------------------------------------------------------- */
/* Zero‑position infrastructure ---------------------------------------------- */
static seq_t *zero_positions = NULL;
static size_t zero_len       = 0;
static size_t zero_cap       = 0;

/* Ensure zero_positions can hold n entries. */
static void _ensure_zero_cap(size_t needed) {
    if (zero_cap < needed) {
        size_t new_cap = zero_cap ? zero_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(zero_positions, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        zero_positions = tmp;
        zero_cap       = new_cap;
    }
}

/* ------------------------------------------------------------------------- */
/* Main sequence function ----------------------------------------------------- */
static idx_t last_processed = 0;        /* n processed so far */
static int64_t S_val = 0;               /* running value of S(n) */

/* Return the n‑th term of A131693 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */

    /* Continue processing until we have at least n zero‐positions. */
    while (zero_len < (size_t)n) {
        ++last_processed;                       /* next n to process */

        uint64_t prime = _get_prime(last_processed + 1);
        seq_t prime_val = (seq_t)prime;

        /* sign = (-1)^last_processed: +1 if even, -1 if odd. */
        int64_t sign = (last_processed & 1) ? -1 : 1;

        S_val = (S_val + (int64_t)prime_val) * sign;

        if (S_val == 0) {
            _ensure_zero_cap(zero_len + 1);
            zero_positions[zero_len++] = (seq_t)last_processed;
        }
    }

    return zero_positions[n - 1];
}
