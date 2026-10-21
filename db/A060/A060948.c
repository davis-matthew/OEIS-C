/*
 * A060948  Intrinsic 11-palindromes: n is an intrinsic k-palindrome if it is
 *          a k-digit palindrome in some base.
 * https://oeis.org/A060948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that are an 11‑digit palindrome in some base.
   We generate all such palindromes in successive bases (2,3,4,…),
   appending the values to a sorted dynamic array.  Since every value
   produced in base b is smaller than any value produced in a larger
   base, the array is intrinsically sorted.  No deduplication is
   necessary because the value ranges of different bases do not
   overlap.  The function returns the nth smallest value (n≥1).  */

#include <stdlib.h>

static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_base = 2;      /* next base to generate values from */

static void append_cache(seq_t v)
{
    if (cache_len == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = v;
}

static void generate_base(uint64_t base)
{
    seq_t base_t = (seq_t)base;
    for (uint64_t d10 = 1; d10 < base; ++d10) {
        for (uint64_t d9 = 0; d9 < base; ++d9) {
            for (uint64_t d8 = 0; d8 < base; ++d8) {
                for (uint64_t d7 = 0; d7 < base; ++d7) {
                    for (uint64_t d6 = 0; d6 < base; ++d6) {
                        for (uint64_t d5 = 0; d5 < base; ++d5) {
                            seq_t val = 0;
                            val = val * base_t + (seq_t)d10;
                            val = val * base_t + (seq_t)d9;
                            val = val * base_t + (seq_t)d8;
                            val = val * base_t + (seq_t)d7;
                            val = val * base_t + (seq_t)d6;
                            val = val * base_t + (seq_t)d5;
                            val = val * base_t + (seq_t)d6;
                            val = val * base_t + (seq_t)d7;
                            val = val * base_t + (seq_t)d8;
                            val = val * base_t + (seq_t)d9;
                            val = val * base_t + (seq_t)d10;
                            append_cache(val);
                        }
                    }
                }
            }
        }
    }
}

static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        generate_base(next_base);
        ++next_base;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n==0; avoid crash */
    ensure_cache(n);
    return cache[n - 1];
}
