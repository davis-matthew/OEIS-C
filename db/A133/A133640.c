/*
 * A133640  List of pairs n,4n, where n is the least unused number so far.
 * https://oeis.org/A133640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(1)=1, a(2)=4.
   For every step pick the least positive integer m that does not yet
   appear in the sequence, output m, then output 4*m.
   Thus the sequence consists of the interleaved pairs (m,4*m). */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* memoised sequence values, capacity as 'seq_cap' */
static seq_t *seq       = NULL;
static size_t seq_cap   = 0;
static size_t seq_len   = 0;

/* bit flags marking numbers that have already appeared */
static uint8_t *used     = NULL;
static size_t used_cap   = 0;

/* next candidate for the minimal unused number m */
static idx_t next_candidate = 1;

/* Grow the sequence array to accommodate 'needed' elements. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap < needed) {
        size_t newcap = seq_cap ? seq_cap << 1 : 16;
        while (newcap < needed) newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
}

/* Grow the used-array so that index 'needed' is valid (inclusive). */
static void ensure_used_cap(size_t needed)
{
    if (used_cap <= needed) {
        size_t newcap = used_cap ? used_cap << 1 : 16;
        while (newcap <= needed) newcap <<= 1;
        uint8_t *newused = (uint8_t *)realloc(used, newcap * sizeof(uint8_t));
        if (newused) {
            /* zero any new slots */
            if (newcap > used_cap)
                memset(newused + used_cap, 0, (newcap - used_cap) * sizeof(uint8_t));
            used    = newused;
            used_cap = newcap;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* index 0 is undefined, return 0 */

    /* generate values until we have at least 'n' terms */
    while (seq_len < (size_t)n) {
        /* find the next unused integer m */
        while (1) {
            if ((size_t)next_candidate >= used_cap)
                ensure_used_cap((size_t)next_candidate);
            if (!used[(size_t)next_candidate]) break;
            ++next_candidate;
        }

        idx_t m = next_candidate;

        /* append m */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = (seq_t)m;

        /* mark m as used */
        used[(size_t)m] = 1;

        /* compute 4*m (as an idx_t) */
        idx_t four_m = m * 4;
        size_t four_m_idx = (size_t)four_m;
        ensure_used_cap(four_m_idx);
        used[four_m_idx] = 1;

        /* append 4*m */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = (seq_t)four_m;

        ++next_candidate;                  /* next search starts at m+1 */
    }

    /* return the requested (1‑based) term */
    return seq[(size_t)n - 1];
}
