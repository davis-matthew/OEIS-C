/*
 * A081648  Integers congruent to 0, 1, 4, 9, 16, 25, 36, 49 or 64 (mod 81)
 *          which are not squares.
 * https://oeis.org/A081648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 319, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = the n‑th positive integer that is ≡ 0,1,4,9,16,25,36,49 or 64 (mod 81)
   and is not a perfect square.  The sequence starts with a(1)=82. */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* 9 quadratic residues modulo 81 */
static const seq_t residues[9] = {0,1,4,9,16,25,36,49,64};

/* true if v mod 81 is one of the residues */
static bool is_residue81(seq_t v)
{
    seq_t r = v % 81;
    for (int i = 0; i < 9; ++i)
        if (r == residues[i]) return true;
    return false;
}

/* integer floor(sqrt(v)) by binary search */
static seq_t isqrt(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = (lo + hi) / 2;
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* true if v is a perfect square */
static bool is_square(seq_t v)
{
    seq_t s = isqrt(v);
    return s * s == v;
}

/* ------- memoisation infrastructure ------------------------------------- */

static seq_t *cache = NULL;   /* array of sequence values */
static size_t capacity = 0;   /* allocated capacity */
static size_t length   = 0;   /* number of values stored */

/* enlarge the cache to hold at least sz elements */
static void ensure_capacity(size_t sz)
{
    if (sz <= capacity) return;
    size_t new_cap = capacity ? capacity * 2 : 8;
    while (new_cap < sz) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t)); /* cast required for C++ */
    cache = tmp;
    capacity = new_cap;
}

/* generate terms until the cache holds at least n values */
static void generate_upto(size_t n)
{
    static seq_t last_checked = 0;   /* number examined so far           */
    for (; length < n; ++last_checked) {
        if (is_residue81(last_checked) && !is_square(last_checked)) {
            ensure_capacity(length + 1);
            cache[length++] = last_checked;
        }
    }
}

/* ------- public function ----------------------------------------------- */

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n < 1 is irrelevant */
    if (n == 0) return 0;
    if ((size_t)n > length) generate_upto((size_t)n);
    return cache[(size_t)n - 1];
}
