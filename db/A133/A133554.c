/*
 * A133554  Lesser of emirps (pairs) with only prime digits (A128388).
 * https://oeis.org/A133554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the lesser of the emirp pairs with only prime digits
 *            (A128388)
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

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

/* Cached terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap   = 0;

/* Current length being examined */
static idx_t current_len = 1;

/* Prime digits used to construct candidates */
static const char prime_digits[4] = {'2', '3', '5', '7'};

/* Recursive generator of numbers of a given length */
static void rec_gen(idx_t len, char *digits, idx_t pos)
{
    if (pos == len) {
        /* Convert digit array to a number */
        seq_t n = 0;
        for (idx_t i = 0; i < len; ++i)
            n = n * 10 + (seq_t)(digits[i] - '0');

        seq_t rev = reverse_digits(n);
        if (rev == n)   /* skip palindromes */
            return;

        if (is_prime_big(n) && is_prime_big(rev)) {
            if (n < rev)
                append_candidate(&cache, &cache_size, &cache_cap, n);
        }
        return;
    }

    for (int k = 0; k < 4; ++k) {
        digits[pos] = prime_digits[k];
        rec_gen(len, digits, pos + 1);
    }
}

/* Generate all numbers of a specific length and collect emirps */
static void generate_len(idx_t len)
{
    char digits[32];            /* enough for all practical lengths */
    rec_gen(len, digits, 0);
}

/* Public function for sequence A133554 */
seq_t a(idx_t n)
{
    /* Lazily compute terms until we have at least n of them */
    while (cache_size < n) {
        ++current_len;
        generate_len(current_len);
    }
    return cache[n - 1];   /* offset 1: a(1) is the first term */
}
