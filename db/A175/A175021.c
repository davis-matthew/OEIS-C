/*
 * A175021  A positive integer k is included if k is not the smallest positive
 *          integer with its particular multiset of run-lengths (of either 0
 *          or 1 considered together) in its binary representation.
 * https://oeis.org/A175021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 189, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer k > 0 such that k is NOT the smallest
   positive integer having its multiset of binary run‑lengths. */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* A run‑length multiset key: up to 64 runs (the maximum for a 64‑bit
   integer).  The run lengths are stored sorted ascending. */
typedef struct {
    uint8_t  len;          /* number of run lengths in data[] */
    uint8_t  data[64];     /* sorted run lengths */
    seq_t    min_val;      /* smallest integer seen with this multiset */
} KeyEntry;

seq_t a(idx_t n)
{
    /* ----------------------------------------------------------------
       Static state that persists across calls
       ---------------------------------------------------------------- */
    static KeyEntry  entries[50000];  /* capacity is generous enough for k up to ~2^64 */
    static size_t    entries_len     = 0;   /* number of distinct keys seen */

    static seq_t    seq[20000];            /* stored sequence terms */
    static size_t   seq_len          = 0;   /* number of terms produced */

    static idx_t    next_k           = 1;   /* next integer to examine */

    /* ----------------------------------------------------------------
       Generate terms until we have at least n of them
       ---------------------------------------------------------------- */
    while (seq_len < (size_t)n) {
        /* Convert next_k to a 64‑bit integer for bit manipulation */
        uint64_t k = (uint64_t)next_k;

        /* ---- 1. compute the sorted multiset of run lengths of k ---- */
        uint8_t   lens[64];
        size_t    lens_cnt = 0;

        /* find most significant bit position */
        int msb = 63;
        while (msb >= 0 && ((k >> msb) & 1ULL) == 0) msb--;
        if (msb < 0) {          /* k == 0, should not happen for positive k */
            ++next_k;
            continue;
        }

        int curbit  = (int)((k >> msb) & 1ULL);
        int run_len = 0;

        for (int j = msb; j >= 0; --j) {
            int bit = (int)((k >> j) & 1ULL);
            if (bit == curbit) {
                ++run_len;
            } else {
                lens[lens_cnt++] = (uint8_t)run_len;
                curbit  = bit;
                run_len = 1;
            }
        }
        lens[lens_cnt++] = (uint8_t)run_len;  /* last run */

        /* sort the lengths (insertion sort, small size) */
        for (size_t i = 1; i < lens_cnt; ++i) {
            uint8_t v = lens[i];
            size_t  j = i;
            while (j > 0 && lens[j - 1] > v) {
                lens[j] = lens[j - 1];
                --j;
            }
            lens[j] = v;
        }

        /* ---- 2. look for an existing key with the same multiset ---- */
        int  found    = 0;
        seq_t min_val = 0;
        for (size_t i = 0; i < entries_len; ++i) {
            if (entries[i].len == lens_cnt &&
                memcmp(entries[i].data, lens, lens_cnt) == 0) {
                found    = 1;
                min_val  = entries[i].min_val;
                break;
            }
        }

        /* ---- 3. act based on whether it's the minimal for this multiset ---- */
        if (found) {
            /* k > min_val ==> k is not the smallest, so add it to the sequence */
            if ((seq_t)k > min_val) {
                seq[seq_len++] = (seq_t)k;
                if (seq_len >= 20000) { /* safety clamp */
                    /* no more space, but n is expected to be smaller */
                    break;
                }
            }
        } else {
            /* new multiset: record its minimum value */
            if (entries_len < 50000) {
                entries[entries_len].len                = (uint8_t)lens_cnt;
                memcpy(entries[entries_len].data, lens, lens_cnt);
                entries[entries_len].min_val            = (seq_t)k;
                ++entries_len;
            }
        }

        ++next_k;
    }

    /* ----------------------------------------------------------------
       Return the requested term (1‑based index)
       ---------------------------------------------------------------- */
    return seq[n - 1];
}
