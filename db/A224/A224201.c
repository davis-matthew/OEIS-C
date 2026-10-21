/*
 * A224201  Number of nX5 0..3 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) counts the number of n×5 arrays over {0,1,2,3}
 * whose rows are unimodal (first non‑decreasing, then non‑increasing)
 * and whose antidiagonals (elements with equal column+row index)
 * are nondecreasing.  The function below computes a(n) for
 * any n ≥ 1, memoising results and evaluating new terms
 * using a simple dynamic‑programming recurrence.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build system; do not define them here. */

/* ---------- data structures ---------- */
typedef struct {
    unsigned char v[5];
} Row;

typedef struct {
    idx_t *data;   /* indices of predecessor rows */
    size_t sz;
    size_t cap;
} Vec;

static idx_t R;                 /* number of admissible rows          */
static Row rows[1024];          /* all rows (≤4⁵=1024)                */
static Vec preds[1024];         /* predecessor lists for each row    */

static seq_t *dp_cur = NULL;    /* DP vector for current length       */
static seq_t *dp_next = NULL;   /* DP vector for next length          */

static seq_t *a_vals = NULL;    /* memoised a(n) values (1‑based)    */
static idx_t a_cap = 0;         /* capacity of a_vals                */
static idx_t computed_n = 0;    /* largest n for which a_vals is filled */

/* ---------- helper functions ---------- */
static inline bool is_unimodal(const unsigned char *v)
{
    for (idx_t pivot = 0; pivot < 5; ++pivot) {
        bool ok = true;
        for (idx_t i = 0; i + 1 < pivot; ++i)
            if (v[i] > v[i + 1]) { ok = false; break; }
        if (!ok) continue;
        for (idx_t i = pivot; i + 1 < 5; ++i)
            if (v[i] < v[i + 1]) { ok = false; break; }
        if (ok) return true;
    }
    return false;
}

static void vec_init(Vec *v)
{
    v->data = NULL;
    v->sz   = 0;
    v->cap  = 0;
}

static void vec_push(Vec *v, idx_t val)
{
    if (v->cap == v->sz) {
        v->cap = v->cap ? v->cap * 2 : 8;
        v->data = (idx_t *)realloc(v->data, v->cap * sizeof(idx_t));
    }
    v->data[v->sz++] = val;
}

/* ---------- initialization ---------- */
static void init_sequence(void)
{
    if (computed_n) return;           /* already initialised */

    /* 1. Enumerate all unimodal rows */
    idx_t cnt = 0;
    for (idx_t mask = 0; mask < 1024; ++mask) {
        unsigned char v[5];
        idx_t t = mask;
        for (idx_t i = 0; i < 5; ++i) { v[i] = t & 3; t >>= 2; }
        if (is_unimodal(v)) {
            for (idx_t i = 0; i < 5; ++i) rows[cnt].v[i] = v[i];
            ++cnt;
        }
    }
    R = cnt;

    /* 2. Initialise predecessor vectors */
    for (idx_t i = 0; i < R; ++i) vec_init(&preds[i]);

    /* 3. Build predecessor lists */
    for (idx_t to = 0; to < R; ++to) {
        const unsigned char *b = rows[to].v;
        for (idx_t from = 0; from < R; ++from) {
            const unsigned char *a = rows[from].v;
            if (a[1] <= b[0] && a[2] <= b[1] &&
                a[3] <= b[2] && a[4] <= b[3]) {
                vec_push(&preds[to], from);
            }
        }
    }

    /* 4. Allocate DP arrays */
    dp_cur  = (seq_t *)malloc(R * sizeof(seq_t));
    dp_next = (seq_t *)malloc(R * sizeof(seq_t));
    for (idx_t i = 0; i < R; ++i) dp_cur[i] = 1;   /* length‑1 arrays */

    /* 5. Allocate result array */
    a_cap = 256;
    a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
    a_vals[0] = 0;   /* unused, offset is 1 */

    /* 6. Compute a(1) */
    seq_t sum = 0;
    for (idx_t i = 0; i < R; ++i) sum += dp_cur[i];
    a_vals[1] = sum;
    computed_n = 1;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    init_sequence();                 /* ensure all structures built   */

    if (n <= computed_n) return a_vals[n];

    /* Expand a_vals array if necessary */
    while (n + 1 > a_cap) {
        a_cap = a_cap ? a_cap * 2 : 1;
        a_vals = (seq_t *)realloc(a_vals, a_cap * sizeof(seq_t));
    }

    /* Compute terms from computed_n+1 up to n */
    for (idx_t len = computed_n + 1; len <= n; ++len) {
        /* Compute dp_next from dp_cur using predecessor lists */
        for (idx_t i = 0; i < R; ++i) dp_next[i] = 0;
        for (idx_t j = 0; j < R; ++j) {
            Vec *v = &preds[j];
            for (size_t k = 0; k < v->sz; ++k)
                dp_next[j] += dp_cur[v->data[k]];
        }

        /* Sum to obtain a(len) */
        seq_t s = 0;
        for (idx_t i = 0; i < R; ++i) s += dp_next[i];
        a_vals[len] = s;

        /* Swap dp_cur and dp_next for next iteration */
        seq_t *tmp = dp_cur; dp_cur = dp_next; dp_next = tmp;
    }

    computed_n = n;
    return a_vals[n];
}
