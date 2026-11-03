/*
 * A061372  Primes having only 0,4,6,8,9 as digits.
 * https://oeis.org/A061372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = the nth prime whose decimal digits are all in {0,4,6,8,9}
 *            * Computation: generate all integers of increasing decimal
 *            length that * contain only digits from {0,4,6,8,9} (with first
 *            digit from {4,6,8,9}), * test each for primality, and keep the
 *            primes in order. All primes found * are stored...
 * Range:     u8 n <= 1, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9904 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Helper: add a prime value to the dynamic array.  Reallocate if needed. */
static void push_prime(seq_t **array, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        if (*cap == 0) *cap = 128;
        else *cap <<= 1;
        *array = mem_resize(seq_t, *array, (*cap * sizeof(seq_t)) / sizeof(seq_t));
    }
    (*array)[(*len)++] = val;
}

/* Recursive generation of all numbers of a fixed length 'k'.
 * pos         – current digit position (0‑based)
 * cur         – number built so far
 * k           – desired length
 * array,len,cap – storage for primes
 */
static void gen_recursive(idx_t pos, seq_t cur, size_t k,
                          seq_t **array, size_t *len, size_t *cap)
{
    if (pos == k) {
        if (is_prime_big(cur))
            push_prime(array, len, cap, cur);
        return;
    }

    static const int first_digits[] = {4, 6, 8, 9};
    static const int other_digits[] = {0, 4, 6, 8, 9};
    const int *digits;
    int dcount;

    if (pos == 0) {
        digits = first_digits;
        dcount = 4;
    } else {
        digits = other_digits;
        dcount = 5;
    }

    for (int i = 0; i < dcount; ++i) {
        seq_t nxt = cur * 10 + digits[i];
        gen_recursive(pos + 1, nxt, k, array, len, cap);
    }
}

/* Generate all numbers of length 'k' (and all their primes).  Results
 * are appended to the static cache. */
static void generate_length(size_t k,
                            seq_t **array, size_t *len, size_t *cap)
{
    gen_recursive(0, 0, k, array, len, cap);
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; outside range */

    /* static cache of found primes */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static size_t gen_len = 1;          /* last fully processed length */

    /* Ensure we have at least n primes */
    while (len < n) {
        size_t k = gen_len + 1;          /* next length to process */
        generate_length(k, &seq, &len, &cap);
        gen_len = k;
    }

    return seq[n - 1];
}
