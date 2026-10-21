/*
 * A065880  Largest positive number that is n times the number of 1's in its
 *          binary expansion, or 0 if no such number exists.
 * https://oeis.org/A065880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 5670, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcnt_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065880
 * a(n) = largest positive integer k such that k = n * popcount(k),
 * or 0 if no such k exists. */

#include <stdint.h>
#include "../utils/popcnt_seq_t.h"
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
#define HAVE_UINT128 1
#else
#define HAVE_UINT128 0
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t n_seq = (seq_t)n;

    /* A safe upper bound for the number of 1's (p).  200 is far above
       the maximum needed for any n <= 2^64. */
    const int MAX_P = 200;

    for (int p = MAX_P; p >= 1; --p) {
        seq_t k;
#if HAVE_UINT128
        unsigned __int128 prod = (unsigned __int128)n_seq * (unsigned __int128)p;
        if (sizeof(seq_t) == 8) {        /* 64‑bit build */
            if (prod > (unsigned __int128)UINT64_MAX) continue;
            k = (unsigned long long)prod;
        } else {                         /* 128‑bit or arbitrary‑precision build */
            k = (seq_t)prod;
        }
#else
        /* Fallback for compilers without __int128 support.  Use 64‑bit
           arithmetic and guard against overflow. */
        unsigned long long n64 = (unsigned long long)n_seq;
        unsigned long long prod = 0;
        int overflow = 0;
        for (int i = 0; i < p; ++i) {
            if (prod > UINT64_MAX - n64) { overflow = 1; break; }
            prod += n64;
        }
        if (overflow) continue;
        k = (seq_t)prod;
#endif
        if (popcnt_seq_t(k) == p) return k;
    }

    return 0;
}
