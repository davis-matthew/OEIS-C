/*
 * A062849  When expressed in base 2 and then interpreted in base 8, is a
 *          multiple of the original number.
 * https://oeis.org/A062849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 734, u32 n <= 1775, u64 n <= 1775,
 *            u128 n <= 1775, big any n
 * Validated: standard, pass, 80 terms through n = 1666 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A062849 – numbers x for which the binary string of x,
   when interpreted as a base‑8 number, is a multiple of x.
   The terms are listed in increasing order, starting with 0 at a(1). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Fallback for systems without __int128 support.
   The algorithm uses repeated doubling to avoid 128‑bit arithmetic. */
static inline uint64_t mul8_mod64(uint64_t a, uint64_t mod)
{
    uint64_t res = 0;
    for (int i = 0; i < 3; ++i)           /* multiply by 8 = 2^3 */
    {
        res = a + a;                      /* doubling */
        if (res >= mod) res -= mod;
        a   = res;
    }
    return res;
}

static inline bool satisfies(uint64_t x)
{
    if (x == 0) return true;
    uint64_t rem  = 0;
    uint64_t mult = 1 % x;                /* 8^0 mod x, always 1 for x>1 */
    uint64_t bits = x;
    while (bits)
    {
        if (bits & 1ULL)
            rem = (rem + mult) % x;
        bits >>= 1ULL;
        mult = mul8_mod64(mult, x);
    }
    return rem == 0;
}

#if defined(__SIZEOF_INT128__)

#else

#endif

/* ---------- Dynamic storage for the sequence ---------- */
static seq_t *seq_vals   = NULL;  /* array of found values          */
static size_t seq_cap    = 0;      /* current capacity of seq_vals   */
static size_t seq_cnt    = 0;      /* number of valid values stored  */
static uint64_t next_val = 0;      /* next integer to test           */

/* ---------- Public sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for calls with n=0 */

    /* Initialise storage on first call. */
    if (seq_vals == NULL)
    {
        seq_cap = 8;
        seq_vals = mem_new(seq_t, seq_cap);
        if (!seq_vals) abort();   /* out of memory */
    }

    /* Generate terms until we have at least n of them. */
    while (seq_cnt < (size_t)n)
    {
        if (satisfies(next_val))
        {
            if (seq_cnt == seq_cap)
            {
                seq_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, seq_vals, seq_cap);
                if (!tmp) abort();
                seq_vals = tmp;
            }
            seq_vals[seq_cnt++] = (seq_t)next_val;
        }
        ++next_val;
    }

    return seq_vals[n - 1];
}
