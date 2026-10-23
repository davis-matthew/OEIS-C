/*
 * A117063  Hexagonal numbers for which the product of the digits is also a
 *          hexagonal number.
 * https://oeis.org/A117063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117063: hexagonal numbers for which the product of the digits
   is also a hexagonal number.  The code generates successive hexagonal
   numbers H_k = k(2k-1) and keeps those whose digit product is itself
   hexagonal (using the property that x is hexagonal iff x=0 or
   8x+1 is a square whose square-root is ≡3 (mod 4)). */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

/* static cache of accepted hexagonal numbers -------------------------------- */
static seq_t *hex_terms = NULL;   /* array of terms */
static size_t hex_cap   = 0;      /* allocated capacity */
static size_t hex_cnt   = 0;      /* number of terms stored */
static idx_t  next_k    = 0;      /* next k to test for H_k */

/* return true if x is a hexagonal number (x=0 or 8x+1 is a square with sqrt≡3 mod4) */
static bool is_hexagonal(seq_t x)
{
    if (x == (seq_t)0) return true;
    seq_t v = (seq_t)8 * x + 1;
    seq_t s = floor_sqrt(v);
    return s * s == v && (s & 3u) == 3u;   /* s ≡ 3 (mod 4) */
}

/* generate hexagonal numbers until at least n terms are stored */
static void ensure_terms(idx_t n)
{
    while (hex_cnt < (size_t)n)
    {
        /* compute next hexagonal number H_k = k(2k-1) */
        seq_t k = (seq_t)next_k;
        seq_t H = k * ((k << 1) - (seq_t)1);

        /* check digit product condition */
        seq_t prod = digit_product(H);
        if (is_hexagonal(prod))
        {
            if (hex_cnt == hex_cap)                /* grow array if needed */
            {
                size_t new_cap = hex_cap ? hex_cap * 2 : 16;
                hex_terms = seq_realloc(hex_terms, hex_cap, new_cap);
                hex_cap = new_cap;
            }
            hex_terms[hex_cnt++] = H;
        }
        ++next_k;
    }
}

/* public function required by the framework --------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;     /* undefined for n=0, return 0 */
    ensure_terms(n);
    return hex_terms[n - 1];
}
