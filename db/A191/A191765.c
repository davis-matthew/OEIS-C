/*
 * A191765  Integers that are a sum of two nonzero triangular numbers and also
 *          the sum of two nonzero square numbers.
 * https://oeis.org/A191765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191765 – numbers that are a sum of two non‑zero triangular
 * numbers and also a sum of two non‑zero squares.
 *
 * Implementation strategy:
 *   * Precompute, on demand, a bitset of all values that can be expressed
 *     as a sum of two positive triangular numbers up to a given LIMIT.
 *   * Similarly precompute a bitset of all values that can be expressed
 *     as a sum of two positive squares up to the same LIMIT.
 *   * The nth term is then found by scanning forward, counting the
 *     numbers that are set in *both* bitsets.
 *   * If the current LIMIT is insufficient to reach the nth term,
 *     the LIMIT is doubled and the bitsets rebuilt.
 *
 * This approach requires only a small byte array per LIMIT and does
 * not rely on static storage of elements whose type may be non‑trivial
 * (e.g., an arbitrary‑precision integer class). It is efficient enough
 * for n up to at least 10 000, staying well below the time budget.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: set bit 'pos' in byte array 'bits'. */
static inline void set_bit(unsigned char *bits, idx_t pos)
{
    bits[pos >> 3] |= (1u << (pos & 7));
}

/* Helper: test bit 'pos' in byte array 'bits'. */
static inline bool get_bit(const unsigned char *bits, idx_t pos)
{
    return (bits[pos >> 3] >> (pos & 7)) & 1u;
}

/* Global state: current limit and its bitsets. */
static idx_t   current_limit  = 0;
static unsigned char *tri_bits = NULL;
static unsigned char *sq_bits  = NULL;

/* Rebuild the two bitsets up to 'limit'. */
static void build_bitsets(idx_t limit)
{
    /* Release old bitsets if any. */
    free(tri_bits);
    free(sq_bits);
    tri_bits = NULL;
    sq_bits  = NULL;

    /* Allocate one byte per 8 numbers. */
    size_t byte_len = (size_t)(limit >> 3) + 1;
    tri_bits = (unsigned char *)calloc(byte_len, 1);
    sq_bits  = (unsigned char *)calloc(byte_len, 1);

    /* Build list of triangular numbers <= limit. */
    idx_t i = 1;
    /* we keep a small array of triangular numbers on the stack */
    /* maximum count ~ sqrt(2*limit) fits easily in the stack for the limits we use. */
    static idx_t tri[4000];          /* 4000 > sqrt(2*10^7) */
    size_t tri_cnt = 0;
    while (1) {
        idx_t t = i * (i + 1) / 2;
        if (t > limit) break;
        tri[tri_cnt++] = t;
        ++i;
    }

    /* Set all sums of two triangular numbers. */
    for (size_t a = 0; a < tri_cnt; ++a) {
        for (size_t b = 0; b < tri_cnt; ++b) {
            idx_t s = tri[a] + tri[b];
            if (s > limit) continue;
            set_bit(tri_bits, s);
        }
    }

    /* Build list of squares <= limit. */
    i = 1;
    static idx_t sq[4000];
    size_t sq_cnt = 0;
    while (1) {
        idx_t s = i * i;
        if (s > limit) break;
        sq[sq_cnt++] = s;
        ++i;
    }

    /* Set all sums of two squares. */
    for (size_t a = 0; a < sq_cnt; ++a) {
        for (size_t b = 0; b < sq_cnt; ++b) {
            idx_t s = sq[a] + sq[b];
            if (s > limit) continue;
            set_bit(sq_bits, s);
        }
    }

    current_limit = limit;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, harmless */

    /* Initialize with a modest limit. */
    if (current_limit == 0)
        build_bitsets(100000);

    /* Search for the nth term. */
    while (true) {
        idx_t count = 0;
        for (idx_t val = 2; val <= current_limit; ++val) {
            if (get_bit(tri_bits, val) && get_bit(sq_bits, val)) {
                ++count;
                if (count == n)
                    return (seq_t)val;
            }
        }
        /* Not enough terms found: double the limit and rebuild. */
        build_bitsets(current_limit * 2);
    }
}
