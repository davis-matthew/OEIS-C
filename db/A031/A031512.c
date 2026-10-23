/*
 * A031512  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 14.
 * https://oeis.org/A031512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031512: numbers k such that the continued fraction for sqrt(k)
   has even period and, after deleting the last term of the period,
   its central term equals 14.
   The function computes the sequence on demand, memoising results
   across calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer floor square root for 64‑bit unsigned integers. */
static uint64_t _isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = UINT64_MAX >> 1;        /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == (__int128)n) return mid;
        if (sq < (__int128)n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

/* Test whether a non‑square integer k satisfies the OEIS condition. */
static bool _good_k(uint64_t k)
{
    uint64_t a0 = _isqrt_u64(k);
    if (a0 * a0 == k) return false;           /* perfect square */

    const size_t MAX_TLEN = 8000;              /* sufficient for all k in range */
    uint64_t terms[MAX_TLEN];
    size_t tlen = 0;

    uint64_t m = 0, d = 1, a = a0;
    while (1) {
        m = (uint64_t)((__int128)d * a - m);
        d = (uint64_t)((k - (__int128)m * m) / d);
        a = (a0 + m) / d;

        if (tlen >= MAX_TLEN)                  /* safety check */
            return false;
        terms[tlen++] = a;

        if (a == 2 * a0) break;                /* period terminates */
    }

    if (tlen % 2 != 0) return false;          /* require even period */

    return terms[tlen / 2 - 1] == 14;          /* central term after deletion */
}

/* Static storage for memoised sequence values. */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;
static uint64_t next_k = 2;                   /* first integer to test */

/* Ensure that seq contains at least 'needed' terms. */
static void _extend_sequence(idx_t needed)
{
    if (seq_len >= (size_t)needed) return;

    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        next_k  = 2;
    }

    while (seq_len < (size_t)needed) {
        while (!_good_k(next_k)) next_k++;     /* skip non‑good k's */

        /* Append the found good k */
        if (seq_len == seq_cap) {              /* enlarge buffer */
            seq_cap *= 2;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)next_k;
        next_k++;                              /* advance for next search */
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* a(0) is undefined */
    _extend_sequence(n);
    return seq[n - 1];
}
