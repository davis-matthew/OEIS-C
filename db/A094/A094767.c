/*
 * A094767  Square spiral of sums of selected preceding terms, starting at 1
 *          (a spiral Fibonacci-like sequence).
 * https://oeis.org/A094767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Σ a(i) over all i< n-1 whose positions are
 *            horizontally, vertically or diagonally adjacent to the position
 *            of a(n-1) (Moore neighborhood). a(1) = 1, a(2) = 1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 43, u64 n <= 87, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build system supplies the following types. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* Determine a grid large enough to contain at least `n` cells. */
    /* Radius r satisfies (2*r+1)^2 >= n.  For typical n (≤10⁴) the
       following linear search is trivial. */
    int64_t r = 0;
    while ((uint64_t)(2 * r + 1) * (uint64_t)(2 * r + 1) < n)
        ++r;

    /* Use a slightly larger grid to avoid boundary checks for neighbours. */
    const int64_t size   = 2 * r + 5;   /* grid size (odd)   */
    const int64_t offset = r + 2;       /* offset to centre   */

    /* Allocate the grid.  Use `new` in C++ and `calloc` in C to get a
       zero‑initialised array which is required for the “empty” cells. */
#ifdef __cplusplus
    seq_t *grid = new seq_t[(size_t)size * (size_t)size]();
#else
    seq_t *grid = calloc((size_t)size * (size_t)size, sizeof(seq_t));
#endif
    if (!grid) return (seq_t)0; /* allocation failure */

    /* Starting position at the centre of the grid. */
    int64_t x = 0, y = 0;
    seq_t last_val = (seq_t)1;
    grid[(y + offset) * size + (x + offset)] = last_val;

    /* Spiral construction parameters. */
    int64_t step_len  = 1;               /* steps in current leg   */
    int64_t step_left = step_len;       /* remaining steps in leg */
    int dir = 0;                         /* 0:right, 1:up, 2:left, 3:down */
    int64_t const dir_x[4] = { 1, 0, -1, 0 };
    int64_t const dir_y[4] = { 0, 1, 0, -1 };

    idx_t count = 1;   /* number of terms computed so far */

    while (count < n) {
        /* Sum values of all already‑filled neighbours (Moore neighbourhood). */
        seq_t adj_sum = (seq_t)0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int64_t nx = x + dx;
                int64_t ny = y + dy;
                int64_t ix = nx + offset;
                int64_t iy = ny + offset;
                /* Skip out‑of‑bounds cells (should not happen with our buffer). */
                if (ix < 0 || iy < 0 || ix >= size || iy >= size) continue;
                seq_t v = grid[iy * size + ix];
                if (v != (seq_t)0) adj_sum += v;
            }
        }

        /* New value to be placed at the next cell. */
        seq_t new_val = last_val + adj_sum;

        /* Advance to the next cell in the spiral. */
        x += dir_x[dir];
        y += dir_y[dir];
        grid[(y + offset) * size + (x + offset)] = new_val;
        last_val = new_val;
        ++count;

        /* Update step counters and change direction when necessary. */
        --step_left;
        if (step_left == 0) {
            dir = (dir + 1) & 3;          /* next direction */
            if (dir % 2 == 0) ++step_len; /* after every two legs */
            step_left = step_len;
        }
    }

    /* Clean up the grid.  Use matching deallocation. */
#ifdef __cplusplus
    delete[] grid;
#else
    free(grid);
#endif

    return last_val;
}
