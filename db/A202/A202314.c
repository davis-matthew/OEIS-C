/*
 * A202314  Number of (n+2)X7 binary arrays avoiding patterns 001 and 100 in
 *          rows and columns.
 * https://oeis.org/A202314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Transition‑matrix DP for binary arrays avoiding * 001 and 100 in
 *            rows and columns. * The implementation counts sequences of (n+2)
 *            rows by a matrix * multiplication over encoded states (pair of
 *            consecutive rows)
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global static data (initialized lazily) */
static bool _init_done = false;
static int _legal_rows[128];
static int _legal_count;
static int _legal_index[128];          /* mapping from row value to index in _legal_rows */
static int _pair_id[128][128];         /* pair ID for two legal rows, -1 if not legal */
static int _pair_row_a[128 * 128];     /* first row of pair    */
static int _pair_row_b[128 * 128];     /* second row of pair   */
static int _pair_count;

static int *_edges_start;              /* start index in edges_dest for each state   */
static int *_edges_deg;                /* number of outgoing transitions per state */
static int *_edges_dest;               /* destination pair IDs                           */

static seq_t *_dp_arr;                 /* DP vector for current number of rows processed */
static seq_t *_newdp_arr;              /* Temporary DP vector used for the next step */

/* --------------------------------------------------------------------- */
/*  Helper to initialise all static data.                                 */
/* --------------------------------------------------------------------- */
static void _init_sequences(void) {
    if (_init_done) return;
    _init_done = true;

    /* --------- 1. Enumerate legal rows (no 001 or 100 in the row) ------- */
    int idx = 0;
    for (int row = 0; row < 128; ++row) {
        bool ok = true;
        for (int j = 0; j <= 4; ++j) {
            int b0 = (row >> j) & 1;
            int b1 = (row >> (j + 1)) & 1;
            int b2 = (row >> (j + 2)) & 1;
            if (b0 == 0 && b1 == 0 && b2 == 1) { ok = false; break; }
            if (b0 == 1 && b1 == 0 && b2 == 0) { ok = false; break; }
        }
        if (ok) {
            _legal_rows[idx] = row;
            _legal_index[row] = idx;
            idx++;
        }
    }
    _legal_count = idx;

    /* --------- 2. Enumerate all pairs of legal rows -------------------- */
    _pair_count = 0;
    for (int i = 0; i < _legal_count; ++i) {
        int a = _legal_rows[i];
        for (int j = 0; j < _legal_count; ++j) {
            int b = _legal_rows[j];
            _pair_id[a][b] = _pair_count;
            _pair_row_a[_pair_count] = a;
            _pair_row_b[_pair_count] = b;
            _pair_count++;
        }
    }

    /* --------- 3. Count outgoing transitions for each state ----------- */
    int *deg = (int *)malloc(_pair_count * sizeof(int));
    if (!deg) {  /* unlikely, but just in case */
        abort();
    }
    memset(deg, 0, _pair_count * sizeof(int));
    const int FULL = 0x7F;              /* 7‑bit mask */
    for (int s = 0; s < _pair_count; ++s) {
        int a = _pair_row_a[s];
        int b = _pair_row_b[s];
        for (int ri = 0; ri < _legal_count; ++ri) {
            int r = _legal_rows[ri];
            /* Check for vertical patterns in columns */
            uint8_t a8 = (uint8_t)a;
            uint8_t b8 = (uint8_t)b;
            uint8_t r8 = (uint8_t)r;
            uint8_t mask1 = (uint8_t)((~a8 & ~b8 & r8) & FULL);
            uint8_t mask2 = (uint8_t)((a8 & ~b8 & (~r8)) & FULL);
            if ((mask1 | mask2) != 0) continue;  /* invalid */
            deg[s]++;                            /* valid triple */
        }
    }
    /* --------- 4. Build CSR (edges_start, edges_dest) --------------- */
    _edges_start = (int *)malloc((_pair_count + 1) * sizeof(int));
    _edges_deg   = (int *)malloc(_pair_count * sizeof(int));
    if (!_edges_start || !_edges_deg) abort();
    _edges_start[0] = 0;
    long long total_edges = 0;
    for (int s = 0; s < _pair_count; ++s) {
        _edges_deg[s] = deg[s];
        total_edges += deg[s];
        _edges_start[s + 1] = (int)(total_edges);
    }
    _edges_dest = (int *)malloc(total_edges * sizeof(int));
    if (!_edges_dest) abort();

    /* ---- 5. Fill the transition destinations ------------------------ */
    for (int s = 0; s < _pair_count; ++s) {
        int a = _pair_row_a[s];
        int b = _pair_row_b[s];
        int pos = _edges_start[s];
        for (int ri = 0; ri < _legal_count; ++ri) {
            int r = _legal_rows[ri];
            uint8_t a8 = (uint8_t)a;
            uint8_t b8 = (uint8_t)b;
            uint8_t r8 = (uint8_t)r;
            uint8_t mask1 = (uint8_t)((~a8 & ~b8 & r8) & FULL);
            uint8_t mask2 = (uint8_t)((a8 & ~b8 & (~r8)) & FULL);
            if ((mask1 | mask2) != 0) continue;  /* invalid */
            _edges_dest[pos++] = _pair_id[b][r];
        }
    }
    free(deg);

    /* --------- 6. Allocate DP arrays ---------------------------------- */
    _dp_arr    = (seq_t *)calloc(_pair_count, sizeof(seq_t));
    _newdp_arr = (seq_t *)calloc(_pair_count, sizeof(seq_t));
    if (!_dp_arr || !_newdp_arr) abort();
}

/* --------------------------------------------------------------------- */
/*  Main function required by the problem. --------------------------------
 *  Computes a(n) for the OEIS sequence A202314.                         */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Sequence is defined for n >= 1 (offset 1) */
    if (n == 0) return 0;

    _init_sequences();

    /* Number of rows in the array */
    size_t rows = (size_t)n + 2;                 /* n+2 rows */
    if (rows < 3) { /* cannot form 3 rows, result is 0 but never happens for n>=1 */
        if (n == 0) return 0;
    }

    /* ---------- 7. Initialise DP with all pairs of first two rows -------- */
    /* reset dp array to zero */
    memset(_dp_arr, 0, _pair_count * sizeof(seq_t));
    for (int i = 0; i < _legal_count; ++i) {
        for (int j = 0; j < _legal_count; ++j) {
            int pid = _pair_id[_legal_rows[i]][_legal_rows[j]];
            _dp_arr[pid] += (seq_t)1;
        }
    }

    /* ---------- 8. Add remaining rows one by one ------------------------ */
    size_t steps = rows > 2 ? rows - 2 : 0;     /* number of new rows to add */
    for (size_t step = 0; step < steps; ++step) {
        memset(_newdp_arr, 0, _pair_count * sizeof(seq_t));
        for (int s = 0; s < _pair_count; ++s) {
            seq_t val = _dp_arr[s];
            if (val == 0) continue;
            int start = _edges_start[s];
            int end   = _edges_start[s + 1];
            for (int p = start; p < end; ++p) {
                int dest = _edges_dest[p];
                _newdp_arr[dest] += val;
            }
        }
        /* swap dp arrays */
        seq_t *tmp = _dp_arr;
        _dp_arr = _newdp_arr;
        _newdp_arr = tmp;
    }

    /* ---------- 9. Sum all final states to obtain the answer ------------- */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < _pair_count; ++s) {
        ans += _dp_arr[s];
    }
    return ans;
}
