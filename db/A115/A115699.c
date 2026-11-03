/*
 * A115699  Primes whose digit reversal is the product of 2 palindromes
 *          greater than 1.
 * https://oeis.org/A115699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth prime p such that rev(p) = x * y with * 1 < x, y
 *            and both x and y are palindromes in base 10
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   ipow_i32, is_palindrome_base, is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_palindrome_base.h"
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *seq_values = NULL;  /* stored terms of the sequence */
static size_t seq_len   = 0;      /* number of terms stored */
static size_t seq_cap   = 0;      /* current capacity of seq_values */
static idx_t  next_candidate = 2; /* next integer to test for primality */

/* test whether a prime p satisfies the definition */
static bool prime_has_palindrome_product(idx_t p)
{
    seq_t revp = reverse_digits_idx(p);                      /* reverse of p */
    if (revp <= (seq_t)1) return false;       /* need factors > 1 */
    seq_t limit = floor_sqrt(revp);
    for (seq_t d = (seq_t)2; d <= limit; ++d) {
        if (revp % d == (seq_t)0) {
            seq_t d2 = revp / d;
            if (d > (seq_t)1 && d2 > (seq_t)1 &&
                is_palindrome_base(d, (idx_t)10) &&
                is_palindrome_base(d2, (idx_t)10)) {
                return true;
            }
        }
    }
    return false;
}

/* enlarge seq_values array to hold at least 'need' elements */
static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
#ifdef __cplusplus
    seq_values = mem_resize(seq_t, seq_values, new_cap);
#else
    seq_values = realloc(seq_values, new_cap * sizeof(seq_t));
#endif
    if (!seq_values) abort();
    seq_cap = new_cap;
}

/* ensure seq_values contains at least n terms */
static void ensure_n_terms(idx_t n)
{
    while (seq_len < (size_t)n) {
        while (!is_prime(next_candidate)) ++next_candidate;
        if (prime_has_palindrome_product(next_candidate)) {
            grow_seq(seq_len + 1);
            seq_values[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for OEIS; harmless */
    ensure_n_terms(n);
    return seq_values[(size_t)n - 1];
}
