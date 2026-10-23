/*
 * A189193  Number of nX6 binary arrays without the pattern 0 0 1 vertically
 *          or antidiagonally.
 * https://oeis.org/A189193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays without the pattern 0 0 1
 *            vertically or antidiagonally
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* A 6‑column binary row is represented by a 6‑bit integer (0…63).
   A state is two consecutive rows, stored in 12 bits:
   bits 11..6 : row r‑2   (top row of the window)
   bits 5..0  : row r‑1   (bottom row of the window)          */
#define STATE_BITS 12
#define NSTATES (1U << STATE_BITS)   /* 4096 */

/* Transition data structures --------------------------------------------------- */
static bool    _trans_init = false;
static uint8_t _trans_cnt[NSTATES];
static uint32_t _trans_offset[NSTATES + 1];          /* indices in _trans_list   */
static uint16_t _trans_list[NSTATES * 64];          /* flattened transition table*/

/* DP memory -------------------------------------------------------------------- */
static seq_t _dp_cur[NSTATES];
static seq_t _dp_next[NSTATES];

/* Build the legal transitions once ------------------------------------------- */
static void _build_transitions(void)
{
    _trans_offset[0] = 0;
    for (uint32_t s = 0; s < NSTATES; ++s) {
        uint32_t prev2 = s >> 6;          /* bits 11..6  : row r‑2 */
        uint32_t prev1 = s & 0x3FU;       /* bits 5..0   : row r‑1 */

        uint16_t    cnt = 0;
        uint32_t base = _trans_offset[s];

        for (uint32_t cur = 0; cur < (1U << 6); ++cur) {
            bool ok = true;

            /* vertical 0 0 1 : same column, consecutive rows */
            for (int c = 0; c < 6; ++c) {
                if ( ((prev2 >> c) & 1U) == 0U &&
                     ((prev1 >> c) & 1U) == 0U &&
                     ((cur    >> c) & 1U) == 1U ) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* antidiagonal 0 0 1 : columns shift +1 each row
               pattern cells: (r‑2,c+2),(r‑1,c+1),(r,c) */
            for (int c = 0; c <= 3; ++c) {
                if ( ((prev2 >> (c + 2)) & 1U) == 0U &&
                     ((prev1 >> (c + 1)) & 1U) == 0U &&
                     ((cur    >>  c      ) & 1U) == 1U ) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;

            /* legal transition: next state = (prev1 << 6) | cur   */
            _trans_list[base + cnt] = (uint16_t)((prev1 << 6) | cur);
            ++cnt;
        }

        _trans_cnt[s]   = cnt;
        _trans_offset[s + 1] = base + cnt;
    }
}

/* --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Fast base cases */
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)4096;

    /* Initialise transitions on first call */
    if (!_trans_init) {
        _build_transitions();
        _trans_init = true;
    }

    seq_t *dp_cur  = _dp_cur;
    seq_t *dp_next = _dp_next;

    /* After two rows every pair of rows is allowed: count = 1 each */
    for (uint32_t s = 0; s < NSTATES; ++s) {
        dp_cur[s] = 1;
    }

    /* Iterate rows 3 … n */
    for (idx_t row = 3; row <= n; ++row) {
        /* clear next array */
        for (uint32_t s = 0; s < NSTATES; ++s) {
            dp_next[s] = 0;
        }

        /* transition */
        for (uint32_t s = 0; s < NSTATES; ++s) {
            seq_t cnt = dp_cur[s];
            if (cnt == 0) continue;
            uint32_t beg = _trans_offset[s];
            uint32_t end = _trans_offset[s + 1];
            for (uint32_t p = beg; p < end; ++p) {
                dp_next[_trans_list[p]] += cnt;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states */
    seq_t result = 0;
    for (uint32_t s = 0; s < NSTATES; ++s) {
        result += dp_cur[s];
    }
    return result;
}
