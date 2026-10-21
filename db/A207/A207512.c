/*
 * A207512  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 25, u64 n <= 58, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207512 – Number of n×6 binary arrays avoiding
 *  horizontal 000 and 011, and vertical 001 and 111.
 *  The function uses a small dynamic‐programming state machine.
 *  It builds all admissible 6‑bit rows and a transition table
 *  for successive rows, then counts compositions of height n.
 */

#include <stdint.h>
#include <stdlib.h>

/*--------------------------------------------------------------------
 *  Static state (initialised on first call)
 *--------------------------------------------------------------------*/
static int _init = 0;                /* initialised flag                */
static uint8_t *_rows = NULL;        /* admissible rows (Rsize entries) */
static int _Rsize = 0;               /* number of admissible rows      */
static uint8_t *_next = NULL;        /* flattened transition lists      */
static uint8_t *_count = NULL;       /* counts of transitions per pair  */
static seq_t *_dp_prev = NULL;       /* DP array for two previous rows */
static seq_t *_dp_cur  = NULL;       /* DP array for current rows      */

/*--------------------------------------------------------------------
 *  Helper to build all admissible rows (horizontal constraints)
 *--------------------------------------------------------------------*/
static void _build_rows(void)
{
    uint8_t tmp[64];
    int sz = 0;
    for (int pat = 0; pat < 64; ++pat) {
        int ok = 1;
        /* check all 4 consecutive triples in the 6‑bit row   */
        for (int c = 0; c <= 3; ++c) {
            int sub = (pat >> (5 - c - 2)) & 7;      /* bits at columns c,c+1,c+2      */
            if (sub == 0  /* 000 */ || sub == 3) {     /* 011 */
                ok = 0;
                break;
            }
        }
        if (ok) {
            tmp[sz++] = (uint8_t)pat;
        }
    }
    _Rsize = sz;
    _rows  = (uint8_t*)malloc(_Rsize * sizeof(uint8_t));
    for (int i = 0; i < sz; ++i) _rows[i] = tmp[i];
}

/*--------------------------------------------------------------------
 *  Helper to build transition table (vertical constraints)
 *--------------------------------------------------------------------*/
static void _build_transitions(void)
{
    size_t totalPairs = (size_t)_Rsize * _Rsize;
    _next  = (uint8_t*)calloc(totalPairs * _Rsize, sizeof(uint8_t));
    _count = (uint8_t*)calloc(totalPairs, sizeof(uint8_t));

    for (int i = 0; i < _Rsize; ++i) {
        for (int j = 0; j < _Rsize; ++j) {
            size_t idx = (size_t)i * _Rsize + j;
            uint8_t ct = 0;
            for (int k = 0; k < _Rsize; ++k) {
                int ok = 1;
                for (int col = 0; col < 6; ++col) {
                    int xb = (_rows[i] >> (5 - col)) & 1;
                    int yb = (_rows[j] >> (5 - col)) & 1;
                    int zb = (_rows[k] >> (5 - col)) & 1;
                    int triple = (xb << 2) | (yb << 1) | zb; /* top to bottom */
                    if (triple == 1 || triple == 7) {        /* 001 or 111 */
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    _next[idx * _Rsize + ct] = (uint8_t)k;
                    ct++;
                }
            }
            _count[idx] = ct;
        }
    }
}

/*--------------------------------------------------------------------
 *  Main sequence function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* initialise static data on first call */
    if (!_init) {
        _build_rows();
        _build_transitions();

        size_t totalPairs = (size_t)_Rsize * _Rsize;
        _dp_prev = (seq_t*)calloc(totalPairs, sizeof(seq_t));
        _dp_cur  = (seq_t*)calloc(totalPairs, sizeof(seq_t));

        _init = 1;
    }

    if (n == 0) return (seq_t)0;              /* offset is 1, undefined for n<1 */

    if (n == 1) return (seq_t)_Rsize;          /* single row */
    if (n == 2) return (seq_t)_Rsize * (seq_t)_Rsize;  /* two independent rows */

    /* n >= 3 : dynamic programming over height */
    size_t totalPairs = (size_t)_Rsize * _Rsize;

    /* initialise for height = 2 : every pair of rows is allowed */
    for (size_t idx = 0; idx < totalPairs; ++idx) {
        _dp_prev[idx] = (seq_t)1;
    }

    for (idx_t step = 3; step <= n; ++step) {
        /* clear current DP */
        for (size_t idx = 0; idx < totalPairs; ++idx) {
            _dp_cur[idx] = (seq_t)0;
        }

        /* transition */
        for (size_t idx = 0; idx < totalPairs; ++idx) {
            seq_t ways = _dp_prev[idx];
            if (!ways) continue;
            int i = idx / _Rsize;          /* first row of the pair */
            int j = idx % _Rsize;          /* second row of the pair */
            uint8_t ct = _count[idx];
            for (uint8_t t = 0; t < ct; ++t) {
                uint8_t k = _next[idx * _Rsize + t];
                size_t nextIdx = (size_t)j * _Rsize + k;
                _dp_cur[nextIdx] += ways;
            }
        }

        /* swap dp arrays for next iteration */
        seq_t* tmp = _dp_prev;
        _dp_prev   = _dp_cur;
        _dp_cur    = tmp;
    }

    /* sum all final states to obtain total number of arrays */
    seq_t total = (seq_t)0;
    for (size_t idx = 0; idx < totalPairs; ++idx) {
        total += _dp_prev[idx];
    }
    return total;
}
