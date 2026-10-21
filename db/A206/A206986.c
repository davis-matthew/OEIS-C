/*
 * A206986  Number of nX7 0..1 arrays avoiding the patterns 0 1 0 or 1 0 1 in
 *          any row, column, diagonal or antidiagonal.
 * https://oeis.org/A206986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of n×7 binary arrays avoiding patterns 010 or 101 in any
 *            row, column, diagonal or antidiagonal. Computed by a
 *            transfer‑matrix of the last two rows
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t uint8_bool;

/* ------------------------------------------------------------------ */
/* Helpers for the model                                             */
/* ------------------------------------------------------------------ */
/* Does a 3‑cell triple (a,b,c) contain the forbidden pattern 010 or 101? */
static inline uint8_bool forbidden_triple(uint8_t a, uint8_t b, uint8_t c)
{
    return (a == c) && (a != b);
}

/* Is a 7‑bit row valid with respect to the horizontal 3‑cell constraint? */
static inline uint8_bool valid_row(uint8_t row)
{
    for (int j = 0; j <= 4; ++j) {
        uint8_t a = (row >> j) & 1;
        uint8_t b = (row >> (j + 1)) & 1;
        uint8_t c = (row >> (j + 2)) & 1;
        if (forbidden_triple(a, b, c))
            return 0;
    }
    return 1;
}

/* Check whether rows a,b,c together satisfy all vertical and diagonal
   constraints.  Horizontal constraint on row c is known to hold. */
static inline uint8_bool triple_ok(uint8_t a, uint8_t b, uint8_t c)
{
    int j;
    /* vertical */
    for (j = 0; j < 7; ++j) {
        uint8_t aa = (a >> j) & 1;
        uint8_t bb = (b >> j) & 1;
        uint8_t cc = (c >> j) & 1;
        if (forbidden_triple(aa, bb, cc))
            return 0;
    }
    /* main diagonal */
    for (j = 2; j < 7; ++j) {
        uint8_t aa = (a >> (j - 2)) & 1;
        uint8_t bb = (b >> (j - 1)) & 1;
        uint8_t cc = (c >> j) & 1;
        if (forbidden_triple(aa, bb, cc))
            return 0;
    }
    /* anti‑diagonal */
    for (j = 0; j < 5; ++j) {
        uint8_t aa = (a >> (j + 2)) & 1;
        uint8_t bb = (b >> (j + 1)) & 1;
        uint8_t cc = (c >> j) & 1;
        if (forbidden_triple(aa, bb, cc))
            return 0;
    }
    return 1;
}

/* ------------------------------------------------------------------ */
/* State representation and transition table                           */
/* ------------------------------------------------------------------ */
typedef struct {
    int *data;      /* list of next row indices */
    size_t len;     /* how many entries */
    size_t cap;     /* allocated capacity */
} TransList;

/* ------------------------------------------------------------------ */
/* The main function                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* Lazy static initialization                                        */
    /* ------------------------------------------------------------------ */
    static int initialized = 0;
    static size_t S = 0;                     /* number of valid rows          */
    static uint8_t *validRows = NULL;        /* array of valid row bitmasks  */
    static int rowIndex[128];                /* mapping from bitmask to index */
    static TransList *trans = NULL;          /* transition table              */
    static seq_t *dp_prev = NULL;            /* DP states for current n-2      */
    static seq_t *dp_curr = NULL;            /* DP states for current n-1      */
    static size_t dp_size = 0;               /* S * S                         */
    static seq_t *cache = NULL;              /* a[1 .. max]                    */
    static size_t cache_cap = 0;             /* allocated cache slots         */
    static size_t cache_len = 0;             /* number of terms stored (max)   */
    static size_t cached_n = 0;              /* highest n that has been computed */

    /* --- Build the list of valid rows -------------------------------- */
    if (!initialized) {
        /* Compute all 7‑bit rows that avoid the horizontal pattern. */
        uint8_t tempRows[128];
        size_t count = 0;
        for (int r = 0; r < 128; ++r) {
            if (valid_row((uint8_t)r)) {
                tempRows[count++] = (uint8_t)r;
            }
        }
        S = count;
        validRows = (uint8_t*)malloc(S * sizeof(uint8_t));
        memcpy(validRows, tempRows, S * sizeof(uint8_t));

        /* Mapping from bitmask to index. */
        for (int i = 0; i < 128; ++i) rowIndex[i] = -1;
        for (size_t i = 0; i < S; ++i) rowIndex[validRows[i]] = (int)i;

        /* Build transition table: for each pair (A,B) list all C that are
           compatible. */
        dp_size = S * S;
        trans = (TransList*)malloc(dp_size * sizeof(TransList));
        for (size_t i = 0; i < dp_size; ++i) {
            trans[i].data = NULL;
            trans[i].len  = 0;
            trans[i].cap  = 0;
        }
        for (int i = 0; i < (int)S; ++i) {
            for (int j = 0; j < (int)S; ++j) {
                size_t pair = (size_t)i * S + j;
                TransList *list = &trans[pair];
                for (int k = 0; k < (int)S; ++k) {
                    if (triple_ok(validRows[i], validRows[j], validRows[k])) {
                        if (list->cap == list->len) {
                            size_t newCap = (list->cap == 0) ? 2 : list->cap * 2;
                            list->data = (int*)realloc(list->data,
                                                      newCap * sizeof(int));
                            list->cap = newCap;
                        }
                        list->data[list->len++] = k;
                    }
                }
            }
        }

        /* Allocate DP arrays.  dp_prev holds states for n = 2 (all ones). */
        dp_prev = (seq_t*)malloc(dp_size * sizeof(seq_t));
        dp_curr = (seq_t*)malloc(dp_size * sizeof(seq_t));
        for (size_t i = 0; i < dp_size; ++i)
            dp_prev[i] = (seq_t)1;
        memset(dp_curr, 0, dp_size * sizeof(seq_t));

        /* Cache for a(n).  We use 1‑based indexing; cache[0] is unused. */
        cache_cap = 10;     /* initial capacity */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 3;      /* we will store a[1] and a[2] at indices 1 and 2   */
        cache[0] = (seq_t)0;                       /* dummy */
        cache[1] = (seq_t)S;                       /* a(1) = number of valid rows    */
        cache[2] = (seq_t)S * (seq_t)S;            /* a(2) = S^2                      */
        cached_n = 2;

        initialized = 1;
    }

    if (n == 0)
        return (seq_t)0;

    /* Ensure we have computed up to n. */
    while (cached_n < (size_t)n) {
        size_t next_n = cached_n + 1;   /* compute a(next_n) */

        /* Transition from dp_prev (state for next_n-2) to dp_curr (next_n-1) */
        memset(dp_curr, 0, dp_size * sizeof(seq_t));

        for (size_t state = 0; state < dp_size; ++state) {
            seq_t val = dp_prev[state];
            if (val == (seq_t)0)
                continue;
            int j = (int)(state % S);   /* second element of the state pair */
            TransList *lst = &trans[state];
            for (size_t t = 0; t < lst->len; ++t) {
                int k = lst->data[t];
                size_t target = (size_t)j * S + k;
                dp_curr[target] += val;
            }
        }

        /* Sum all dp_curr values to obtain a(next_n) */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < dp_size; ++i)
            sum += dp_curr[i];

        /* Store in cache, extending if necessary */
        if (next_n >= cache_cap) {
            size_t newCap = cache_cap * 2;
            if (newCap <= next_n) newCap = next_n + 1;
            cache = (seq_t*)realloc(cache, newCap * sizeof(seq_t));
            cache_cap = newCap;
        }
        cache[next_n] = sum;

        /* Prepare for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;

        cached_n = next_n;
    }

    return cache[n];
}
