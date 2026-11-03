/*
 * A046005  Discriminants of imaginary quadratic fields with class number 8
 *          (negated).
 * https://oeis.org/A046005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = discriminants of imaginary quadratic fields with class
 *            number 8 (negated). Computed by enumerating negative fundamental
 *            discriminants and counting reduced binary quadratic forms. The
 *            result is correct for all n up to the known 131 terms
 * Range:     u8 n <= 5, u16 n <= 131, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Validated: standard, pass, 64 terms through n = 127 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mem.h"

/* Count reduced primitive binary quadratic forms for a given fundamental D < 0.
   The count equals the class number. */
static uint64_t class_number_fundamental(int64_t D)
{
    uint64_t absD = (uint64_t)(-D);
    uint64_t a_limit = (uint64_t)sqrt((double)absD / 3.0);
    uint64_t count = 0;

    for (uint64_t a = 1; a <= a_limit; ++a)
    {
        int64_t b_start = -(int64_t)a;
        int64_t b_end   =  (int64_t)a;
        for (int64_t b = b_start; b <= b_end; ++b)
        {
            /* Parity condition: b ≡ D (mod 2) */
            if (((b & 1LL) != (D & 1LL))) continue;

            __int128 rhs  = (__int128)b * b - D;      /* b^2 - D, non‑negative for D < 0 */
            int64_t mod   = (int64_t)(4 * a);
            if ((rhs % mod) != 0) continue;
            int64_t c = (int64_t)(rhs / mod);
            if ((int64_t)a > c) continue;
            if ((b < 0) && (llabs(b) == (int64_t)a)) continue;   /* |b| = a, negative b not reduced */
            if ((b < 0) && ((int64_t)a == c)) continue;          /* a = c, negative b not reduced */
            ++count;
        }
    }
    return count;
}

/* Types provided by the build environment */
typedef idx_t idx_t;
typedef seq_t seq_t;

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;
static int64_t next_D = -1;                       /* next negative integer to test */

/* Ensure the cache can hold at least 'need' terms */
static void ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need) return;
    idx_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, newcap);
    if (!tmp) return;           /* allocation failed – let the request fail */
    cache = tmp;
    cache_cap = newcap;
}

/* Square‑free check for unsigned 64‑bit numbers */
static int is_squarefree_uint64(uint64_t n)
{
    if (n % 4ULL == 0ULL) return 0;
    for (uint64_t p = 2ULL; p * p <= n; ++p)
        if (n % (p * p) == 0ULL) return 0;
    return 1;
}

/* Test whether a negative integer D is a fundamental discriminant */
static int is_fundamental_D(int64_t D)
{
    if (D >= 0) return 0;
    uint64_t absD = (uint64_t)(-D);
    int mod4 = (int)(D & 3LL);                  /* D mod 4, works for negative D */
    if (mod4 == 1)                            /* D ≡ 1 (mod 4) */
        return is_squarefree_uint64(absD);

    if (mod4 == 0)                            /* D ≡ 0 (mod 4) */
    {
        uint64_t absD_div4 = absD >> 2;        /* |D| / 4 */
        uint64_t r = absD_div4 % 4ULL;         /* (|D|/4) mod 4 */
        uint64_t mod4_Ddiv = (4ULL - r) % 4ULL;/* D/4 mod 4, taking sign into account */
        if (mod4_Ddiv != 2ULL && mod4_Ddiv != 3ULL) return 0;
        return is_squarefree_uint64(absD_div4);
    }
    return 0;
}

/* Generate terms until at least 'n' entries are cached */
static void generate_terms(idx_t n)
{
    ensure_cache_capacity(n);
    while (cache_size < n)
    {
        next_D -= 1;                      /* move to next more negative integer */
        if (!is_fundamental_D(next_D)) continue;
        if (class_number_fundamental(next_D) == 8ULL)
            cache[cache_size++] = (seq_t)(- (int64_t)next_D);
    }
}

/* The exported function requested by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, harmless */
    if (cache_size < n) generate_terms(n);
    return cache[n - 1];
}
