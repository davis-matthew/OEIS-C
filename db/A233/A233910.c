/*
 * A233910  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 30, and no
 *          two adjacent values equal.
 * https://oeis.org/A233910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A233910
   Computation: dynamic programming on 56 states (ordered pairs (x,y) with 0≤x,y≤7, x≠y).
   Transition from state (x,y) to (x',y') iff
       x≠x',   y≠y',   and
       (x-y)²+(x'-y')²+(x-x')²+(y-y')² = 30.
   a(n) counts sequences of length n+1 rows that satisfy the transition and
   the no‑consecutive‑equal rule (automatic for two columns).  This is an
   explicit recurrence, no closed form is known.  The code memoises all
   a(k) with k ≤ current maximum.                       */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* State representation and constants                                   */
typedef struct { int x; int y; } state_t;
#define STATE_COUNT 56

/* --------------------------------------------------------------------- */
/* Global static data for the DP                                       */
static bool   _impl_initialized = false;
static int    _impl_nb_neighbors[STATE_COUNT][STATE_COUNT];
static int    _impl_deg[STATE_COUNT];
static seq_t  _impl_curr[STATE_COUNT];
static seq_t * _impl_cache = NULL;
static size_t _impl_cache_cap = 0;
static idx_t  _impl_max_n = 0;

/* --------------------------------------------------------------------- */
/* Initialise state space and first step (computes a(1))                 */
static void _impl_init(void)
{
    state_t states[STATE_COUNT];
    int s_idx = 0;

    /* Enumerate all ordered pairs (x,y) with x != y, 0 ≤ x,y ≤ 7 */
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (x != y)
                states[s_idx++] = (state_t){x, y};

    /* Build adjacency lists */
    for (int i = 0; i < STATE_COUNT; ++i) {
        int xi = states[i].x, yi = states[i].y;
        int deg = 0;
        for (int j = 0; j < STATE_COUNT; ++j) {
            if (i == j) continue;                 /* distinct rows */
            int xj = states[j].x, yj = states[j].y;

            /* No equal values vertically or horizontally */
            if (xi == xj) continue;
            if (yi == yj) continue;

            int d1 = xi - yi;     /* horizontal of row i   */
            int d2 = xj - yj;     /* horizontal of row j   */
            int d3 = xi - xj;     /* vertical (col 1)      */
            int d4 = yi - yj;     /* vertical (col 2)      */
            int sum = d1*d1 + d2*d2 + d3*d3 + d4*d4;

            if (sum == 30)
                _impl_nb_neighbors[i][deg++] = j;
        }
        _impl_deg[i] = deg;
    }

    /* Initialise current vector: one way for each row of length 1 */
    for (int i = 0; i < STATE_COUNT; ++i)
        _impl_curr[i] = (seq_t)1;

    /* First transition: compute a(1) and set curr for length 2 rows */
    seq_t next[STATE_COUNT] = {0};
    for (int u = 0; u < STATE_COUNT; ++u) {
        seq_t val = _impl_curr[u];
        for (int k = 0; k < _impl_deg[u]; ++k) {
            int v = _impl_nb_neighbors[u][k];
            next[v] += val;
        }
    }

    seq_t a1 = 0;
    for (int i = 0; i < STATE_COUNT; ++i)
        a1 += next[i];

    /* Allocate cache for a(1) (indices 0..1) */
    _impl_cache_cap = 4;   /* room for index 0 (unused) and 1 and maybe 2 */
    _impl_cache = (seq_t *)malloc(_impl_cache_cap * sizeof(seq_t));
    if (_impl_cache == NULL) abort();
    for (size_t i = 0; i < _impl_cache_cap; ++i) _impl_cache[i] = 0;
    _impl_cache[1] = a1;

    /* Replace current vector with the result after step 1 */
    for (int i = 0; i < STATE_COUNT; ++i)
        _impl_curr[i] = next[i];

    _impl_max_n = 1;
    _impl_initialized = true;
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                            */
seq_t a(idx_t n)
{
    /* Lazy initialization on first call */
    if (!_impl_initialized)
        _impl_init();

    /* Return cached value if already computed */
    if (n <= _impl_max_n)
        return _impl_cache[n];

    /* Extend the DP until we have computed up to n */
    while (_impl_max_n < n) {
        seq_t next[STATE_COUNT] = {0};
        for (int u = 0; u < STATE_COUNT; ++u) {
            seq_t val = _impl_curr[u];
            for (int k = 0; k < _impl_deg[u]; ++k) {
                int v = _impl_nb_neighbors[u][k];
                next[v] += val;
            }
        }

        /* Sum all ways for this length (a(max_n+1)) */
        seq_t total = 0;
        for (int i = 0; i < STATE_COUNT; ++i)
            total += next[i];

        /* Resize cache if necessary */
        if (_impl_max_n + 1 >= _impl_cache_cap) {
            size_t new_cap = (_impl_max_n + 2) * 2;
            seq_t *new_cache = (seq_t *)realloc(_impl_cache, new_cap * sizeof(seq_t));
            if (!new_cache) abort();
            for (size_t i = _impl_cache_cap; i < new_cap; ++i)
                new_cache[i] = 0;
            _impl_cache = new_cache;
            _impl_cache_cap = new_cap;
        }

        _impl_cache[_impl_max_n + 1] = total;

        /* Move to next state */
        for (int i = 0; i < STATE_COUNT; ++i)
            _impl_curr[i] = next[i];

        ++_impl_max_n;
    }

    return _impl_cache[n];
}
