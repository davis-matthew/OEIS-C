/*
 * A236862  Characteristic function for A236842 (A234742): a(n) = 1, if n is a
 *          result of "upward" remultiplication (GF(2)[X] -> N) of some
 *          number, 0 otherwise.
 * https://oeis.org/A236862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1; for n>1, a(n)=1 if A091225(n)=1 or there exists a
 *            proper factor d (1<d<n) such that a(d)=a(n/d)=1, otherwise
 *            a(n)=0. (A091225(n) is 1 iff the binary polynomial of n is
 *            irreducible over GF(2).)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Helper: bit length of a 64‑bit integer                               */
/* --------------------------------------------------------------------- */
static inline int bitlen_uint64(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 0;
}

/* --------------------------------------------------------------------- */
/*  Helper: test whether n (as a binary polynomial) is irreducible over */
/*          GF(2).  Works correctly up to at least 10 000.                 */
/* --------------------------------------------------------------------- */
static inline bool is_irreducible_uint64(uint64_t n)
{
    if (n <= 1) return false;          /* 0 or 1 are not considered irreducible */
    int deg = bitlen_uint64(n) - 1;
    if (deg <= 1) return true;         /* X or X+1 are irreducible */

    for (int d = 1; d <= deg / 2; ++d) {
        uint64_t start = 1ULL << d;
        uint64_t end   = (1ULL << (d + 1)) - 1;
        for (uint64_t g = start; g <= end; ++g) {
            uint64_t f = n;
            while (true) {
                int bl_f = bitlen_uint64(f);
                int bl_g = bitlen_uint64(g);
                if (bl_f < bl_g) break;
                f ^= g << (bl_f - bl_g);
            }
            if (f == 0) return false;  /* g divides n */
        }
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Caches for memoisation                                               */
/* --------------------------------------------------------------------- */
static seq_t *a_cache = NULL;            /* 0: unknown, 1: value 0, 2: value 1 */
static size_t a_cache_cap = 0;

static int8_t *irred_cache = NULL;       /* -1: unknown, 0: false, 1: true */
static size_t irred_cap = 0;

/* --------------------------------------------------------------------- */
/*  Ensure caches large enough for index n                                 */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
    while ((size_t)n >= new_cap) new_cap <<= 1;

    seq_t *ptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!ptr) abort();                     /* allocation failed */
    for (size_t i = a_cache_cap; i < new_cap; ++i) ptr[i] = (seq_t)-1;
    a_cache = ptr;
    a_cache_cap = new_cap;

    if (new_cap > irred_cap) {
        int8_t *iptr = (int8_t *)realloc(irred_cache, new_cap * sizeof(int8_t));
        if (!iptr) abort();
        for (size_t i = irred_cap; i < new_cap; ++i) iptr[i] = -1;
        irred_cache = iptr;
        irred_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (a_cache[n] != (seq_t)-1) return a_cache[n];

    seq_t result;
    if (n == 0 || n == 1) {
        result = (seq_t)1;
    } else {
        bool irr = false;
        if (irred_cache[n] != -1) {
            irr = irred_cache[n] != 0;
        } else {
            irr = is_irreducible_uint64((uint64_t)n);
            irred_cache[n] = irr ? 1 : 0;
        }

        if (irr) {
            result = (seq_t)1;
        } else {
            bool found = false;
            for (idx_t d = 2; d * d <= n; ++d) {
                if (n % d == 0) {
                    seq_t ad = a(d);
                    seq_t an_d = a(n / d);
                    if (ad && an_d) { found = true; break; }
                }
            }
            result = found ? (seq_t)1 : (seq_t)0;
        }
    }

    a_cache[n] = result;
    return result;
}
