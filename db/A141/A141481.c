/*
 * A141481  Square spiral of sums of selected preceding terms, starting at 1.
 * https://oeis.org/A141481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 149, u64 n <= 478,
 *            u128 n <= 961, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n>1, a(n) is the sum of the values in the 8
   neighboring cells already filled when a new cell is visited in a
   clockwise square spiral starting with 1 at the center. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static const int dx[4]   = { 1, 0, -1, 0 };
static const int dy[4]   = { 0, 1,  0, -1 };
static const int ndx[8]  = { -1, -1, -1,  0, 0, 1, 1, 1 };
static const int ndy[8]  = { -1,  0,  1, -1, 1, -1, 0, 1 };

/* State for the spiral construction */
typedef struct {
    seq_t *seq;          /* array of sequence values */
    size_t seq_len;      /* number of values computed */
    size_t seq_cap;      /* capacity of seq array */

    seq_t *grid;         /* 2‑D grid stored row‑major */
    int width;           /* grid size (square, width=height) */
    int offset;          /* index of the centre cell */

    int x, y;            /* coordinates of the current cell (0,0 is centre) */
    int dir;             /* current direction: 0=right,1=up,2=left,3=down */
    int step_len;        /* number of steps to take in current direction */
    int dir_change;      /* turns taken since last step_len increment */
} SpiralState;

static SpiralState state = {0};

/* Allocate or grow the sequence array to hold at least 'need' elements. */
static void ensure_seq_capacity(size_t need)
{
    if (state.seq == NULL) {
        state.seq_cap = need > 8 ? need * 2 : 8;
        state.seq = (seq_t *)calloc(state.seq_cap, sizeof(seq_t));
    } else if (need > state.seq_cap) {
        state.seq_cap *= 2;
        state.seq = (seq_t *)realloc(state.seq, state.seq_cap * sizeof(seq_t));
    }
}

/* Expand the grid so that coordinates (x,y) fit; copy existing data. */
static void enlarge_grid_if_needed(int x, int y)
{
    int need = (abs(x) > state.offset) ? abs(x) : abs(y);
    if (need <= state.offset)
        return;

    int new_offset = need;
    int new_width  = 2 * new_offset + 1;

    seq_t *new_grid =
        (seq_t *)calloc((size_t)new_width * new_width, sizeof(seq_t));
    for (int gy = 0; gy < state.width; ++gy) {
        for (int gx = 0; gx < state.width; ++gx) {
            seq_t val = state.grid[gy * state.width + gx];
            if (val) {
                int px = gx - state.offset;
                int py = gy - state.offset;
                int nx = px + new_offset;
                int ny = py + new_offset;
                size_t idx = ((size_t)ny * (size_t)new_width) + (size_t)nx;
                new_grid[idx] = val;
            }
        }
    }
    free(state.grid);
    state.grid = new_grid;
    state.width = new_width;
    state.offset = new_offset;
}

/* Compute the next deﬁned number in the spiral and append it to seq. */
static void compute_next(void)
{
    int dir = state.dir;
    for (int step = 0; step < state.step_len; ++step) {
        state.x += dx[dir];
        state.y += dy[dir];

        enlarge_grid_if_needed(state.x, state.y);

        seq_t sum = 0;
        for (int k = 0; k < 8; ++k) {
            int nx = state.x + ndx[k];
            int ny = state.y + ndy[k];
            int gx = nx + state.offset;
            int gy = ny + state.offset;
            if (gx >= 0 && gx < state.width && gy >= 0 && gy < state.width) {
                size_t idx = ((size_t)gy * (size_t)state.width) +
                             (size_t)gx;
                seq_t neigh = state.grid[idx];
                if (neigh)
                    sum += neigh;
            }
        }
        /* store the value in the grid */
        size_t idx =
            ((size_t)(state.y + state.offset) * (size_t)state.width) +
            (size_t)(state.x + state.offset);
        state.grid[idx] = sum;

        /* append to the sequence array */
        size_t new_len = state.seq_len + 1;
        ensure_seq_capacity(new_len);
        state.seq[state.seq_len] = sum;
        state.seq_len = new_len;
    }

    /* rotate direction */
    state.dir = (state.dir + 1) % 4;
    state.dir_change++;
    if ((state.dir_change % 2) == 0)
        state.step_len++;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but return 0 */

    if (state.seq == NULL) {
        /* initialise with a single centre cell */
        state.width = 3;
        state.offset = 1;
        state.grid = (seq_t *)calloc((size_t)state.width * state.width,
                                     sizeof(seq_t));
        int mid = state.offset;
        state.grid[mid * state.width + mid] = 1;

        state.seq = (seq_t *)calloc(8, sizeof(seq_t));
        state.seq[0] = 1;
        state.seq_len = 1;
        state.seq_cap = 8;

        state.x = 0;
        state.y = 0;
        state.dir = 0;
        state.step_len = 1;
        state.dir_change = 0;
    }

    while (state.seq_len < (size_t)n)
        compute_next();

    return state.seq[n - 1];
}
