/*
 * A236578  The number of tilings of a 7 X (3n) floor with 1 X 3 trominoes.
 * https://oeis.org/A236578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A236578: The number of tilings of a 7 × (3n) rectangle with 1×3 trominoes.
   Computed via a transfer‑matrix DP on all admissible column states
   (14‑bit masks).  The DP uses O(n · S) time where S is the number of
   reachable states (≈ few hundred).  The function works for any
   seq_t and idx_t supplied by the build. */

#include <stdlib.h>
#include <string.h>

/* Maximum possible 14‑bit keys (2^14) */
#define MAX_STATES (1 << 14)

/* --------------------------------------------------------------------
   Data structures for state graph
   -------------------------------------------------------------------*/
static int16_t  state_id[MAX_STATES];          /* key → state index (-1 unvisited) */
static uint16_t state_key[MAX_STATES];         /* key of a state by index */
static uint16_t **trans_adj;                   /* adjacency lists: target state indices */
static size_t   *trans_len;                    /* number of outgoing edges per state */

/* --------------------------------------------------------------------
   DFS to enumerate all tilings of a single column given the current
   and next masks.  Resulting transitions are pairs (next, add) where
   *next* is the occupancy of the following column (cur+1) and
   *add* is the occupancy of the column after that (cur+2).
   -------------------------------------------------------------------*/
typedef struct { uint16_t next; uint16_t add; } Res;

static void dfs_fill(int row,
                     uint16_t cur, uint16_t next, uint16_t add,
                     Res *out, size_t *out_len)
{
    if (row == 7) {                      /* finished current column */
        out[(*out_len)++] = (Res){next, add};
        return;
    }
    if (cur & (1 << row)) {               /* cell already occupied */
        dfs_fill(row + 1, cur, next, add, out, out_len);
        return;
    }

    /* Place a vertical tromino (covers rows row,row+1,row+2 in this column) */
    if (row <= 4 &&
        !(cur & (1 << (row + 1))) &&
        !(cur & (1 << (row + 2)))) {
        uint16_t cur2 = cur | (1 << row) | (1 << (row + 1)) | (1 << (row + 2));
        dfs_fill(row + 1, cur2, next, add, out, out_len);
    }

    /* Place a horizontal tromino (covers same row in next two columns) */
    if (!(next & (1 << row)) && !(add & (1 << row))) {
        uint16_t next2 = next | (1 << row);
        uint16_t add2 = add | (1 << row);
        dfs_fill(row + 1, cur, next2, add2, out, out_len);
    }
}

/* Comparator for sorting Res structs by (next, add) */
static int cmp_res(const void *a, const void *b)
{
    const Res *ra = (const Res *)a;
    const Res *rb = (const Res *)b;
    if (ra->next != rb->next) return ra->next < rb->next ? -1 : 1;
    if (ra->add  != rb->add  ) return ra->add  < rb->add  ? -1 : 1;
    return 0;
}

/* --------------------------------------------------------------------
   Build the state graph once (lazily on first call)
   -------------------------------------------------------------------*/
static int graph_built = 0;
static size_t start_index = 0;
static size_t state_count = 0;

static void build_state_graph(void)
{
    memset(state_id, 0xFF, sizeof(state_id)); /* -1 for all */
    trans_adj = (uint16_t **)malloc(MAX_STATES * sizeof(uint16_t *));
    trans_len = (size_t   *)malloc(MAX_STATES * sizeof(size_t));
    if (!trans_adj || !trans_len) return;     /* allocation failed */

    /* BFS queue for states to process */
    uint16_t state_queue[MAX_STATES];
    size_t head = 0, tail = 0;

    /* Start state: no pending cells */
    uint16_t start_key = 0;                 /* cur=0, next=0 */
    state_id[start_key] = (int16_t)0;
    state_key[0] = start_key;
    state_queue[tail++] = start_key;
    state_count = 1;
    start_index = 0;

    /* Temporary array for DFS results */
    Res res_temp[512];

    while (head < tail) {
        uint16_t key = state_queue[head++];
        size_t idx = (size_t)state_id[key];

        uint16_t cur_m = key >> 7;
        uint16_t next_m = key & 0x7F;

        size_t res_len = 0;
        dfs_fill(0, cur_m, next_m, 0, res_temp, &res_len);

        /* Sort and deduplicate transitions */
        qsort(res_temp, res_len, sizeof(Res), cmp_res);
        size_t uniq = 0;
        for (size_t i = 0; i < res_len; ++i) {
            if (i == 0 ||
                res_temp[i].next != res_temp[i-1].next ||
                res_temp[i].add  != res_temp[i-1].add) {
                res_temp[uniq++] = res_temp[i];
            }
        }

        /* Allocate adjacency list for this state */
        trans_adj[idx] = (uint16_t *)malloc(uniq * sizeof(uint16_t));
        trans_len[idx] = uniq;
        for (size_t i = 0; i < uniq; ++i) {
            uint16_t target_key = (res_temp[i].next << 7) | res_temp[i].add;
            int16_t target_id = state_id[target_key];
            if (target_id == -1) {
                target_id = (int16_t)state_count++;
                state_id[target_key] = target_id;
                state_key[target_id] = target_key;
                state_queue[tail++] = target_key;
            }
            trans_adj[idx][i] = (uint16_t)target_id;
        }
    }
    graph_built = 1;
}

/* --------------------------------------------------------------------
   Main sequence function
   -------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!graph_built) build_state_graph();
    if (!graph_built) return (seq_t)0;      /* allocation failure */

    /* Number of columns is 3 n.  Use uint64_t for loop counter. */
    uint64_t columns = (uint64_t)n * 3ULL;
    if (columns == 0) return (seq_t)1;     /* a(0) = 1 */

    /* DP arrays for current and next column states */
    seq_t *dp_cur = (seq_t *)calloc(state_count, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(state_count, sizeof(seq_t));
    if (!dp_cur || !dp_next) return (seq_t)0;       /* allocation failure */

    dp_cur[start_index] = (seq_t)1;

    for (uint64_t step = 0; step < columns; ++step) {
        /* reset next array */
        memset(dp_next, 0, state_count * sizeof(seq_t));

        for (size_t s = 0; s < state_count; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            size_t out = trans_len[s];
            uint16_t *targets = trans_adj[s];
            for (size_t t = 0; t < out; ++t) {
                dp_next[targets[t]] += val;
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    seq_t result = dp_cur[start_index];

    free(dp_cur);
    free(dp_next);
    return result;
}
