/*
 * A098387  Prime(n)+Log2(n), where Log2=A000523.
 * https://oeis.org/A098387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(n) + floor(log2(n))
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Prime(1) = 2;  log2(n) = integer part of the base‑2 logarithm of n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- helper: floor(log2(n)) for unsigned 64‑bit values ---- */
static inline idx_t log2_floor(idx_t n)
{
    if (n == 0)
        return 0;
#if defined(__GNUC__) || defined(__clang__)
    return 63 - __builtin_clzll((unsigned long long)n);
#else
    idx_t r = 0;
    while (n >>= 1) ++r;
    return r;
#endif
}

/* ---- lazy prime generator (trial division) ---- */
static idx_t *prime_list   = NULL;  /* primes as idx_t (≤ 64‑bit) */
static size_t primes_len   = 0;     /* number of primes stored     */
static size_t primes_cap   = 0;     /* current capacity of the array */
static idx_t  last_checked = 1;     /* last integer examined for primality */

/* Ensure that prime_list contains at least 'need' primes. */
static void ensure_primes(idx_t need)
{
    if (primes_len >= need)
        return;

    /* Rough upper bound for nth prime (for n≥6). */
    if (need < 6)
        need = 6; /* minimal table for small n */

    /* Resize prime_list if needed. */
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 256;
        idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(*tmp));
        if (!tmp) /* allocation failed – abort with zeroes */
            return;
        prime_list = tmp;
        primes_cap = new_cap;
    }

    /* Generate primes until we have enough. */
    while (primes_len < need) {
        last_checked++;
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            idx_t p = prime_list[i];
            if (p * p > last_checked)
                break;
            if (last_checked % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_len >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(prime_list, new_cap * sizeof(*tmp));
                if (!tmp) return;
                prime_list = tmp;
                primes_cap = new_cap;
            }
            prime_list[primes_len++] = last_checked;
        }
    }
}

/* Return the nth prime (1‑based) as seq_t. */
static seq_t nth_prime(idx_t n)
{
    ensure_primes(n);
    if (n <= primes_len)
        return (seq_t)prime_list[n - 1];
    return (seq_t)0; /* should not happen */
}

/* ---- required sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1 – safe fallback */

    seq_t ln = (seq_t)log2_floor(n);
    seq_t p  = nth_prime(n);
    return p + ln;
}
