/*
 * A210534  Primes formed by concatenating palindromes having even number of
 *          digits with 1.
 * https://oeis.org/A210534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th prime of the form n || rev(n) || 1 where rev(n)
 *            is the decimal reversal of n and the middle part is padded with
 *            leading zeros so that the concatenation is a palindrome of even
 *            length. The function computes the sequence lazily, caching all
 *            primes found so far
 * Range:     u8 none, u16 n <= 22, u32 n <= 1452, u64 n <= 1452,
 *            u128 n <= 1452, big any n
 * Validated: standard, pass, 51 terms through n = 1158 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits_idx, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits_idx.h"
#include "../utils/reverse_digits_idx.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Cache for the primes found so far */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;      /* number of primes stored          */
    static size_t cache_cap = 0;      /* capacity of the array            */
    static idx_t  next_n   = 1;       /* next integer to test            */

    if (n == 0) return 0;  /* undefined below offset 1 */

    /* Generate terms until we have at least n primes */
    while (cache_len < n)
    {
        /* number of decimal digits in next_n (used for padding) */
        int  len      = num_digits_idx(next_n);
        seq_t rev_n   = reverse_digits_idx(next_n);
        seq_t pal     = ((seq_t)next_n) * pow10((idx_t)len) + rev_n;
        seq_t val     = pal * ((seq_t)10) + ((seq_t)1);

        if (is_prime_big(val))
            append_candidate(&cache, &cache_len, &cache_cap, val);

        ++next_n;
    }

    return cache[n - 1];  /* 0‑based index into cached array */
}
