/*
 * A217568  Rows of the 8 magic squares of order 3 and magic sum 15,
 *          lexicographically sorted.
 * https://oeis.org/A217568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, nonn, full, tabf
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   apply_map, compare_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the flattened entries of the eight 3×3 magic squares with magic sum 15,
   sorted lexicographically and concatenated row‑wise.  The sequence is finite
   (length 72) and a(n) is the n‑th element of this list. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/apply_map.h"
#include "../utils/compare_squares.h"

seq_t a(idx_t n) {
    static seq_t squares[8][9];
    static size_t square_count = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Base 3×3 Lo Shu magic square (numbers 1..9, sum 15). */
        const int base_int[9] = {8, 1, 6, 3, 5, 7, 4, 9, 2};
        seq_t base[9];
        for (int i = 0; i < 9; ++i) base[i] = (seq_t)base_int[i];

        /* Permutation maps for the 8 symmetries of the square. */
        static const int map_id[9]     = {0,1,2,3,4,5,6,7,8};
        static const int map_rot90[9]  = {6,3,0,7,4,1,8,5,2};
        static const int map_rot180[9] = {8,7,6,5,4,3,2,1,0};
        static const int map_rot270[9] = {2,5,8,1,4,7,0,3,6};
        static const int map_ref_vert[9]={2,1,0,5,4,3,8,7,6};
        static const int map_ref_horiz[9]={6,7,8,3,4,5,0,1,2};
        static const int map_ref_main[9]={0,3,6,1,4,7,2,5,8};
        static const int map_ref_anti[9]={8,5,2,7,4,1,6,3,0};

        static const int *maps[8] = {
            map_id, map_rot90, map_rot180, map_rot270,
            map_ref_vert, map_ref_horiz, map_ref_main, map_ref_anti
        };

        seq_t tmp[9];

        for (int t = 0; t < 8; ++t) {
            apply_map(tmp, base, maps[t]);

            /* Check for duplicates. */
            bool seen = false;
            for (size_t s = 0; s < square_count; ++s) {
                bool equal = true;
                for (int i = 0; i < 9; ++i) {
                    if (squares[s][i] != tmp[i]) {
                        equal = false;
                        break;
                    }
                }
                if (equal) { seen = true; break; }
            }
            if (!seen) {
                for (int i = 0; i < 9; ++i) squares[square_count][i] = tmp[i];
                ++square_count;
            }
        }

        /* Sort the unique squares lexicographically. */
        qsort(squares, square_count, sizeof squares[0], compare_squares);
        initialized = true;
    }

    if (n == 0) return (seq_t)0;  /* Not defined for n < 1 */

    size_t idx = (size_t)(n - 1);
    size_t max = square_count * 9;  /* 72 for the standard set */

    if (idx >= max) return (seq_t)0;  /* Out of bounds – return 0 */

    return squares[idx / 9][idx % 9];
}
