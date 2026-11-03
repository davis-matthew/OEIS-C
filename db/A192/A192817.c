/*
 * A192817  Numbers that are coprime to their 9's complement.
 * https://oeis.org/A192817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 6151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9900 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the nth positive integer x such that gcd(x,
   9's complement of x) = 1, where 9's complement of x is (10^k - 1) - x
   with k = number of decimal digits of x.  Multiples of 3 never belong
   to the sequence.  The function generates terms on demand and caches
   them for subsequent calls. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Local helper: count decimal digits of a seq_t value. */
static idx_t digits_seq(seq_t x)
{
    idx_t d = 0;
    if (x == 0) return 1;
    while (x > 0) {
        x /= 10;
        ++d;
    }
    return d;
}

/* Static storage for the sequence values. */
static seq_t *terms = NULL;
static size_t size  = 0;
static size_t cap   = 0;
static seq_t last_checked = 0U;

/* Ensure the cache has at least 'need' terms. */
static void ensure_cache(idx_t need)
{
    if (size >= (size_t)need) return;

    /* Grow the cache as needed. */
    while (size < (size_t)need) {
        last_checked += 1;                     /* next candidate number */

        /* Compute number of digits of last_checked. */
        idx_t k = digits_seq(last_checked);

        /* 9's complement: (10^k - 1) - last_checked. */
        seq_t p10k = pow10(k);
        p10k -= 1;                             /* 10^k - 1 */
        seq_t comp = p10k - last_checked;

        if (gcd(last_checked, comp) == 1) {
            if (size == cap) {                  /* allocate more space */
                cap = cap ? cap * 2 : 64;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[size++] = last_checked;        /* store valid term */
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1, but handle 0 safely */
    ensure_cache(n);
    return terms[n - 1];
}
