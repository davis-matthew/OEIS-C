/*
 * A183788  Half the number of (n+1)X6 0..2 arrays with no 2X2 subblock having
 *          sum 4.
 * https://oeis.org/A183788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½ × (number of (n+1)×6 arrays over {0,1,2} with no 2×2
 *            sub‑block having entry‑sum 4) (computed by a transfer–matrix DP
 *            on the 3^6 possible rows)
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_alloc_array.h"

#define STATES 729   /* 3^6 different rows */
#define WIDTH  6     /* fixed width of the array */

/* adjacency list (encoding which rows can follow each other) */
static int adjacency[STATES][STATES];
static uint16_t neigh_cnt[STATES];

/* pre‑computed base‑3 representation of each row */
static uint8_t state_digits[STATES][WIDTH];

/* DP vectors for the current and next row heights */
static seq_t *cur_vec  = NULL;
static seq_t *next_vec = NULL;

/* number of rows that have already been processed (>=1) */
static idx_t max_rows_done = 0;

/*---------------------------------------------------------------*/
/* Build the adjacency matrix: which pairs of rows can be
   consecutive without forming a forbidden 2×2 block.
*/
static void build_matrix(void)
{
    /* convert each integer 0..STATES-1 to its 6‑digit base‑3 pattern */
    for (idx_t s = 0; s < STATES; ++s) {
        idx_t tmp = s;
        for (int c = WIDTH - 1; c >= 0; --c) {
            state_digits[s][c] = (uint8_t)(tmp % 3);
            tmp /= 3;
        }
    }

    /* for every pair of rows test all 5 overlapping 2×2 blocks */
    for (idx_t r = 0; r < STATES; ++r) {
        uint16_t cnt = 0;
        for (idx_t s = 0; s < STATES; ++s) {
            int OK = 1;
            for (int j = 0; j < WIDTH - 1; ++j) {
                int sum = state_digits[r][j] + state_digits[r][j+1]
                        + state_digits[s][j] + state_digits[s][j+1];
                if (sum == 4) { OK = 0; break; }
            }
            if (OK) adjacency[r][cnt++] = (int)s;
        }
        neigh_cnt[r] = cnt;
    }
}

/*---------------------------------------------------------------*/
/* Allocate the DP vectors on first use.  The seq_alloc_array
   routine initializes each element with the value corresponding to
   seq_t's default constructor (zero for built‑in or big‑int types).
*/
static void init_vectors(void)
{
    if (!cur_vec) {
        cur_vec  = seq_alloc_array(STATES);
        next_vec = seq_alloc_array(STATES);
        if (!cur_vec || !next_vec) abort();   /* allocation failure */

        /* each row can start an array in exactly one way */
        for (idx_t i = 0; i < STATES; ++i) cur_vec[i] = 1;

        max_rows_done = 1;
        build_matrix();
    }
}

/*---------------------------------------------------------------*/
/* Extend the DP to at least target_rows rows (height).  The
   function is idempotent: repeated calls with a larger target
   simply compute the difference.
*/
static void extend_to(idx_t target_rows)
{
    init_vectors();

    while (max_rows_done < target_rows) {
        /* reset next vector */
        memset(next_vec, 0, sizeof(seq_t) * STATES);

        /* count transitions */
        for (idx_t r = 0; r < STATES; ++r) {
            seq_t cnt = cur_vec[r];
            if (cnt == 0) continue;
            uint16_t c = neigh_cnt[r];
            for (uint16_t k = 0; k < c; ++k) {
                int s = adjacency[r][k];
                next_vec[s] += cnt;
            }
        }

        /* swap current and next */
        seq_t *tmp = cur_vec;
        cur_vec = next_vec;
        next_vec = tmp;

        ++max_rows_done;
    }
}

/*---------------------------------------------------------------*/
/* Public sequence function, external linkage. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    idx_t target_rows = n + 1;           /* height of the array is (n+1)×6 */
    extend_to(target_rows);

    /* total number of admissible arrays of that size */
    seq_t total = 0;
    for (idx_t i = 0; i < STATES; ++i) total += cur_vec[i];

    /* the OEIS sequence is one half of that number */
    return total / 2;
}
