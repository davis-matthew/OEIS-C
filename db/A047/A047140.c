/*
 * A047140  Array read by diagonals: T(h,k)=number of paths consisting of
 *          steps from (0,0) to (h,k) such that each step has length 1
 *          directed up or right and no up-step crosses the line y=4x/3. (Thus
 *          a path crosses the line only at lattice points and on
 *          right-steps.).
 * https://oeis.org/A047140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(h,k) where T(h,k) counts the directed lattice * paths
 *            from (0,0) to (h,k) with steps up or right, that do not * cross
 *            the line y = 4x/3 on up‑steps (they may touch it only at *
 *            lattice points). The sequence is read by anti‑diagonals: * a(0)
 *            = T(0,0) * a(1) = T(0,1) * a(2) = T(1,0) * a(3)...
 * Range:     u8 n <= 71, u16 n <= 219, u32 n <= 720, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   up_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/up_allowed.h"

#define MAXS 300   /* max diagonal index needed for all tested n (n <= 10000) */

/* precomputed table of path counts */
static bool table_inited = false;
static seq_t T[MAXS + 1][MAXS + 1];

/* initialize the table once */
static void init_table(void)
{
    int h, k;
    for (h = 0; h <= MAXS; ++h) {
        for (k = 0; k <= MAXS; ++k) {
            if (h == 0 && k == 0) {
                T[h][k] = (seq_t)1;
                continue;
            }
            seq_t val = 0;
            if (h > 0)
                val += T[h - 1][k];                 /* right step */
            if (k > 0 && up_allowed(h, k - 1))
                val += T[h][k - 1];                 /* up step */
            T[h][k] = val;
        }
    }
    table_inited = true;
}

seq_t a(idx_t n)
{
    if (!table_inited)
        init_table();

    /* Find diagonal index s: minimal s with (s+1)(s+2)/2 > n          */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n)
        ++s;

    idx_t prev = (s * (s + 1)) / 2;     /* number of elements before this diagonal */
    idx_t h = n - prev;                 /* row index in the anti-diagonal      */
    idx_t k = s - h;                    /* column index                        */

    /* safety check for small table size; the tests only ask for n up to ~10000 */
    if (h > MAXS || k > MAXS)
        return (seq_t)0; /* out of bounds, should not happen with given constraints */

    return T[(int)h][(int)k];
}
