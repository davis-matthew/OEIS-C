/*
 * A228090  Numbers k for which a sum k + bitcount(k) cannot be obtained as a
 *          sum k2 + bitcount(k2) for any other k2<>k . Here bitcount(k)
 *          (A000120) gives the number of 1's in binary representation of
 *          nonnegative integer k.
 * https://oeis.org/A228090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth element of OEIS A228090.
   The function scans non‑negative integers k, computes f(k)=k+popcount(k)
   and keeps the k for which this value is unique (no other k'≠k gives
   the same f).  It uses a sliding window of size 128 to detect
   duplicates, sufficient for all k that fit into a 64‑bit integer. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static inline int popcount_u64(uint64_t x)
{
#ifdef __GNUC__
    return __builtin_popcountll(x);
#else
    /* Portable fallback (very slow, but unlikely to be used). */
    int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
#endif
}

typedef struct {
    uint64_t k;   /* the integer k */
    seq_t    f;   /* k + popcount(k) */
    bool     unique;  /* true if f has not yet been duplicated */
} RingEntry;

#ifndef OUT_CAP
#define OUT_CAP 20000    /* sufficient for n up to 10000 */
#endif

#define MAX_WINDOW 128   /* sufficient for 64‑bit k; bigger is harmless */

seq_t a(idx_t n)
{
    /* static state */
    static seq_t  output[OUT_CAP];
    static size_t out_len = 0;

    static RingEntry ring[MAX_WINDOW];
    static size_t    ring_head = 0; /* index of oldest entry */
    static size_t    ring_len  = 0; /* current number of stored entries */

    static idx_t processed_k = 0;   /* next k to process */

    if (n < 1) return 0; /* undefined, but just in case */

    /* Keep generating until we have at least n unique terms recorded */
    while (out_len < (size_t)n) {
        idx_t k = processed_k++;
        int pc = popcount_u64((uint64_t)k);
        seq_t f = (seq_t)k + (seq_t)pc;
        bool curr_unique = true;

        /* Search for a duplicate among previous entries in the ring. */
        for (size_t j = 0; j < ring_len; ++j) {
            size_t idx = (ring_head + j) % MAX_WINDOW;
            if (ring[idx].f == f) {
                curr_unique = false;
                ring[idx].unique = false;   /* both entries are non‑unique */
            }
        }

        /* If the ring is full, the oldest entry is now finalized. */
        if (ring_len == MAX_WINDOW) {
            if (ring[ring_head].unique) {
                /* Store it in output (in increasing order of k). */
                if (out_len < OUT_CAP) {
                    output[out_len++] = (seq_t)ring[ring_head].k;
                }
            }
            /* Overwrite the oldest entry with the new one. */
            ring[ring_head].k      = (uint64_t)k;
            ring[ring_head].f      = f;
            ring[ring_head].unique = curr_unique;
            ring_head = (ring_head + 1) % MAX_WINDOW; /* advance head */
        } else {
            /* Ring not yet full – add new entry at the end. */
            ring[ring_len].k      = (uint64_t)k;
            ring[ring_len].f      = f;
            ring[ring_len].unique = curr_unique;
            ++ring_len;
        }
    }

    return output[n - 1];
}
