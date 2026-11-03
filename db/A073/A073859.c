/*
 * A073859  a(1)=1 and a(n>1)=min{primes up to n-1 | a(1)+a(2)+...+a(n-1)+a(n)
 *          is a cube}.
 * https://oeis.org/A073859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = 1; for n > 1, a(n) is the smallest prime p such that
 *            a(1)+a(2)+…+a(n‑1)+p is a perfect cube. The cumulative sum after
 *            n−1 terms is always a perfect cube, so we can compute a(n) as p
 *            = (m³ − S) where m = ⌊cuberoot(S)⌋ + 1 and S is the previous sum
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: full, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Memoised sequence values and cumulative sum */
static seq_t *values = NULL;          /* 1‑based indexing of a(i) */
static size_t vals_size = 0;          /* allocated size in elements */
static idx_t  maxi = 0;               /* largest index computed so far */
static seq_t  sum = 0;                /* Σ_{k=1}^{maxi} a(k) */

/* Ensure values array is large enough for index n (1‑based). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < vals_size) return;
    size_t new_size = (size_t)n + 1;          /* keep index 0 unused */
    if (values == NULL) {
        values = mem_new(seq_t, new_size);
    } else {
        values = mem_resize(seq_t, values, new_size);
    }
    if (values == NULL) {          /* allocation failure – bad news, but
                                    * in the contest environment this
                                    * never happens. */
        return;
    }
    /* zero the newly allocated slots */
    for (size_t i = vals_size; i < new_size; ++i) values[i] = 0;
    vals_size = new_size;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* undefined – guard */

    /* Allocate storage for required index. */
    ensure_capacity(n);

    /* Return already computed value. */
    if (n <= maxi) return values[n];

    /* Initialise first term if necessary. */
    if (maxi == 0) {
        values[1] = (seq_t)1;
        sum = (seq_t)1;
        maxi = (idx_t)1;
    }

    /* Compute terms from maxi+1 up to n. */
    for (idx_t i = maxi + 1; i <= n; ++i) {
        /* Current sum S = a(1)+…+a(i‑1) is a perfect cube,
           namely (i‑1)³.  Compute the next cube (i³).        */
        seq_t m = floor_cube_root(sum) + (seq_t)1;   /* m = floor(cuberoot(S))+1  */
        seq_t cube = m * m * m;                     /* cube = m³ (next perfect cube) */
        seq_t candidate = cube - sum;               /* p = cube – S */

        /* Search for the smallest prime p.  Since m grows slowly,
           a tiny loop suffices; the sequence terminates at n = 5. */
        while (!is_prime_big(candidate)) {
            ++m;
            cube = m * m * m;
            candidate = cube - sum;
        }

        values[i] = candidate;
        sum += candidate;
        maxi = i;
    }

    return values[n];
}
