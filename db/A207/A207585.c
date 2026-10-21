/*
 * A207585  Number of nX4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 53, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n×4 0..1 arrays avoiding
 *  horizontally: 000, 001
 *  vertically:   001, 111
 *
 * The function uses a transfer‑matrix DP over the 10 admissible row
 * patterns.  For arbitrary‑precision seq_t the array is allocated
 * by malloc with an explicit cast, which is valid in both C11 and
 * C++17.  The routine is memoised so that repeated calls in any
 * order are O(n) in total.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* number of admissible 4‑bit rows (no 000 / 001) */
#define ROWS 10
/* states are pairs of successive rows */
#define STATES (ROWS * ROWS)

/* admissible rows in binary (4 bits) */
static const unsigned char rowbits[ROWS] = {
    4, 5, 6, 7,      /* 0100, 0101, 0110, 0111 */
    10, 11, 12, 13,  /* 1010, 1011, 1100, 1101 */
    14, 15           /* 1110, 1111 */
};

/* transition table tr[i][j][k] = 1 if i,j,k can occur consecutively */
static unsigned char tr[ROWS][ROWS][ROWS];
static bool tr_inited = false;

/* dynamic cache for computed values */
static seq_t *a_cache   = NULL;
static size_t cache_cap = 0;
static idx_t  max_computed = 0;

/* DP arrays: number of ways to end a configuration with last two rows i,j */
static seq_t dp_prev[STATES];
static seq_t dp_curr[STATES];

/* Initialise transition table once (C++ safe) */
static void init_trans(void)
{
    if (tr_inited) return;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            for (int k = 0; k < ROWS; ++k) {
                unsigned char ok = 1;
                for (int col = 0; col < 4; ++col) {
                    unsigned a = (rowbits[i] >> col) & 1;
                    unsigned b = (rowbits[j] >> col) & 1;
                    unsigned c = (rowbits[k] >> col) & 1;
                    if ((a == 0 && b == 0 && c == 1) ||
                        (a == 1 && b == 1 && c == 1))
                        { ok = 0; break; }
                }
                tr[i][j][k] = ok;
            }
    tr_inited = true;
}

/* Initialise DP for n = 2 (any pair of allowed rows).  */
static void init_dp(void)
{
    for (int s = 0; s < STATES; ++s) dp_prev[s] = 0;
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp_prev[i * ROWS + j] = (seq_t)1;
    /* dp_curr will be zeroed on first use */
}

/* Grow the cache array as needed.  Uses explicit cast for malloc/realloc. */
static void grow_cache(idx_t n)
{
    if (n <= cache_cap) return;

    size_t new_cap = (size_t)n;
    seq_t *tmp;
    if (a_cache == NULL) {
        tmp = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
    } else {
        tmp = (seq_t*)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    }
    if (!tmp) abort();        /* allocation failure */
    a_cache = tmp;
    for (idx_t i = cache_cap + 1; i <= n; ++i)
        a_cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* out of bounds */
    grow_cache(n);

    /* Base terms  */
    if (a_cache[1] == (seq_t)0) a_cache[1] = (seq_t)10;
    if (n == 1) return a_cache[1];

    if (a_cache[2] == (seq_t)0) a_cache[2] = (seq_t)100;
    if (n == 2) return a_cache[2];

    init_trans();
    if (max_computed < 2) {           /* initialise DP for n=2 once */
        init_dp();
        max_computed = 2;
    }

    /* Compute missing terms up to n */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        for (int s = 0; s < STATES; ++s) dp_curr[s] = (seq_t)0;

        /* extend one more row */
        for (int s = 0; s < STATES; ++s) {
            seq_t ways = dp_prev[s];
            if (!ways) continue;
            int i = s / ROWS;
            int j = s % ROWS;
            for (int k = 0; k < ROWS; ++k)
                if (tr[i][j][k]) {
                    int ns = j * ROWS + k;
                    dp_curr[ns] += ways;
                }
        }

        /* sum all ending states to get a(cur) */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < STATES; ++s) sum += dp_curr[s];
        a_cache[cur] = sum;

        /* prepare for next iteration */
        for (int s = 0; s < STATES; ++s) dp_prev[s] = dp_curr[s];
    }

    max_computed = n;
    return a_cache[n];
}
