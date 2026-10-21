/*
 * A206717  Numbers matched to polynomials divisible by x^2+x+1.
 * https://oeis.org/A206717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer whose binary expansion contains an
 *            equal * number of 1‑bits in each residue class modulo 3 (i.e.,
 *            the counts of 1s in * exponent positions ≡0,1,2 (mod 3) are
 *            equal and non‑zero). This is equivalent * to numbers that can be
 *            written as 2^a + 2^b + 2^c with a≡0, b≡1, c≡2...
 * Range:     u8 n <= 27, u16 n <= 4031, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* cache of pre‑computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_scan = 0;

/* enlarge the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* cannot grow further – abort */
    cache = tmp;
    cache_cap = new_cap;
}

/* test whether x has equal counts of 1‑bits in the three residue classes modulo 3 */
static int is_valid(idx_t x)
{
    int c0 = 0, c1 = 0, c2 = 0;
    int pos = 0;
    while (x) {
        if (x & 1ULL) {
            switch (pos % 3) {
                case 0: ++c0; break;
                case 1: ++c1; break;
                case 2: ++c2; break;
            }
        }
        x >>= 1ULL;
        ++pos;
    }
    return (c0 == c1 && c1 == c2 && c0 > 0);
}

/* ------------------------------------------------------------ */
/* public sequence function                                         */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – undefined for n=0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        ++next_scan;                     /* next candidate integer */
        if (is_valid(next_scan)) {        /* it satisfies the requirement */
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)next_scan;
        }
    }
    return cache[n - 1];
}
