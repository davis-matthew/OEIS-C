/*
 * A237063  Number of (n+1)X(4+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+1)×5 arrays over {0,1,2} for which the * quantity
 *            max + upper‑median + min – lower‑median of every 2×2 subblock *
 *            is the same for all subblocks. The algorithm builds a directed
 *            graph * on the 3^5 = 243 possible row vectors; an edge i→j
 *            exists if the pair * of rows satisfies the requi...
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   block_S
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/block_S.h"

typedef struct {
    int *items;
    int len;
    int cap;
} list_t;

#define ROWS 243          /* 3^5 possible rows */
#define COLS 5
#define S_VALUES 5        /* possible values of the block quantity */

/* Static adjacency lists for each s and each row i */
static list_t adj[S_VALUES][ROWS];
static int row_digits[ROWS][COLS];
static int adjacency_built = 0;

/* Initialize a dynamic list. */
static void list_init(list_t *lst)
{
    lst->len = 0;
    lst->cap = 8;
    lst->items = (int *)malloc(lst->cap * sizeof(int));
}

/* Append an integer to a dynamic list. */
static void list_append(list_t *lst, int val)
{
    if (lst->len == lst->cap) {
        int newcap = lst->cap << 1;
        int *newptr = (int *)realloc(lst->items, newcap * sizeof(int));
        if (newptr) {
            lst->items = newptr;
            lst->cap = newcap;
        }
    }
    lst->items[lst->len++] = val;
}

/* Build all adjacency lists once. */
static void build_adjacency(void)
{
    if (adjacency_built) return;
    adjacency_built = 1;

    /* Generate all row digit vectors. */
    for (int r = 0; r < ROWS; ++r) {
        int x = r;
        for (int c = 0; c < COLS; ++c) {
            row_digits[r][c] = x % 3;
            x /= 3;
        }
    }

    /* Initialise lists */
    for (int s = 0; s < S_VALUES; ++s)
        for (int i = 0; i < ROWS; ++i)
            list_init(&adj[s][i]);

    /* Build edges */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            int first = -1;
            bool ok = true;
            for (int col = 0; col < COLS - 1; ++col) {
                int val = block_S(row_digits[i][col], row_digits[i][col + 1],
                                  row_digits[j][col], row_digits[j][col + 1]);
                if (col == 0)
                    first = val;
                else if (val != first) {
                    ok = false;
                    break;
                }
            }
            if (ok && first >= 0 && first < S_VALUES) {
                list_append(&adj[first][i], j);
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    build_adjacency();

    /* Working vectors */
    seq_t old_states[S_VALUES][ROWS];
    seq_t new_states[S_VALUES][ROWS];

    /* Initialise: start from any row. */
    for (int s = 0; s < S_VALUES; ++s)
        for (int i = 0; i < ROWS; ++i)
            old_states[s][i] = (seq_t)1;

    /* Perform n transitions (walks of length n). */
    for (idx_t step = 0; step < n; ++step) {
        for (int s = 0; s < S_VALUES; ++s) {
            /* Zero new states */
            for (int i = 0; i < ROWS; ++i)
                new_states[s][i] = (seq_t)0;

            /* Transition */
            for (int i = 0; i < ROWS; ++i) {
                seq_t from = old_states[s][i];
                if (from == (seq_t)0) continue;
                list_t *lst = &adj[s][i];
                for (int k = 0; k < lst->len; ++k) {
                    int j = lst->items[k];
                    new_states[s][j] = new_states[s][j] + from;
                }
            }

            /* Copy new to old by elementwise assignment (important for big int types) */
            for (int i = 0; i < ROWS; ++i)
                old_states[s][i] = new_states[s][i];
        }
    }

    /* Sum all ending states over all s */
    seq_t result = (seq_t)0;
    for (int s = 0; s < S_VALUES; ++s) {
        for (int i = 0; i < ROWS; ++i)
            result = result + old_states[s][i];
    }
    return result;
}
