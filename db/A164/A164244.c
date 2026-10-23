/*
 * A164244  Number of binary strings of length n with equal numbers of 00101
 *          and 01001 substrings.
 * https://oeis.org/A164244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164244 – Number of binary strings of length n with equal numbers of 00101 and 01001 substrings.
    The function counts strings for arbitrary n ≥ 0 using dynamic programming.
    The state consists of the last four bits (16 possibilities) and the cumulative
    difference (#00101 – #01001).  The difference ranges from –(n‑4) to +(n‑4);
    its zero value corresponds to diffIndex = n‑4.  The DP propagates one bit
    at a time and uses only addition.  Complexity: O(n²) – about 16·(n‑4)²
    elementary operations for n ≤ 10 000.  The routine works for any seq_t
    provided by the build (big‑int, 64‑bit, etc.).  No global state is needed.  */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Pre‑computed transition tables for each state and next bit. */
static int dest[2][16];
static int shift_val[2][16];
static bool tables_ready = false;

/* Initialise the transition tables once. */
static void init_tables(void)
{
    if (tables_ready) return;
    for (int s = 0; s < 16; ++s) {
        for (int b = 0; b < 2; ++b) {
            /* 5‑bit pattern formed by shifting state and adding next bit */
            int five = ((s << 1) & 0x1F) | b;
            dest[b][s]   = ((s << 1) & 0x0F) | b;   /* last four bits of new pattern */
            shift_val[b][s] = (five == 0b00101) ? 1
                            : (five == 0b01001) ? -1
                            : 0;
        }
    }
    tables_ready = true;
}

seq_t a(idx_t n)
{
    init_tables();

    /* Handle n < 4: all strings have zero substrings of length 5. */
    if (n < 4) {
        seq_t res = 1;
        for (idx_t i = 0; i < n; ++i) res *= 2;
        return res;
    }

    /* Number of steps beyond the first 4 bits. */
    size_t steps     = (size_t)(n - 4);
    size_t diffSize  = 2 * steps + 1;     /* number of difference values      */
    size_t offset    = steps;            /* index of difference zero        */
    size_t stride    = diffSize;         /* stride between states in arrays */
    size_t total     = 16 * diffSize;    /* total entries in an array       */

    /* Allocate two DP arrays and zero them. */
    seq_t *curr = (seq_t *)calloc(total, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(total, sizeof(seq_t));
    if (!curr || !next) {
        /* Allocation failed – return zero (should not happen). */
        free(curr); free(next);
        return 0;
    }

    /* All 16 possible prefixes of length 4 contribute one way to start. */
    for (int s = 0; s < 16; ++s) curr[s * stride + offset] = 1;

    /* Current range of difference indices: [-L, L] with centre offset. */
    size_t startIdx = offset;
    size_t endIdx   = offset;

    /* Main DP loop: each iteration adds one more bit to the string. */
    for (idx_t len = 4; len < n; ++len) {
        /* New range after appending a bit: [-L-1, L+1] */
        size_t newStart = startIdx - 1;
        size_t newEnd   = endIdx + 1;

        /* Zero the part of 'next' that will receive data. */
        for (int s = 0; s < 16; ++s) {
            seq_t *np = next + s * stride;
            for (size_t i = newStart; i <= newEnd; ++i) np[i] = 0;
        }

        /* Propagate from current to next. */
        for (int s = 0; s < 16; ++s) {
            const seq_t *cp = curr + s * stride;
            seq_t *np0 = next + dest[0][s] * stride;
            seq_t *np1 = next + dest[1][s] * stride;

            for (size_t i = startIdx; i <= endIdx; ++i) {
                seq_t val = cp[i];
                if (val == 0) continue;

                /* Append bit 0 */
                size_t idx0 = (size_t)((int)i + shift_val[0][s]);
                np0[idx0] += val;

                /* Append bit 1 */
                size_t idx1 = (size_t)((int)i + shift_val[1][s]);
                np1[idx1] += val;
            }
        }

        /* Prepare for next iteration. */
        seq_t *tmp = curr; curr = next; next = tmp;
        startIdx = newStart;
        endIdx   = newEnd;
    }

    /* Sum over all states at difference zero. */
    seq_t result = 0;
    for (int s = 0; s < 16; ++s) result += curr[s * stride + offset];

    /* Clean up. */
    free(curr);
    free(next);
    return result;
}
