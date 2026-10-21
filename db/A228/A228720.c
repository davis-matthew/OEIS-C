/*
 * A228720  Number of partitions in the first n compositions of j, according
 *          with the ordering of A228525, if 1<=n<=2^(j-1).
 * https://oeis.org/A228720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct partitions that appear, in
 *            lexicographic order by first part ascending, among the first n
 *            compositions of the integer j, where j is the minimal integer
 *            with 2^(j‑1) ≥ n
 * Range:     u8 n <= 27, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#define MAX_J 65

/* Precomputed prefix arrays: prefix[j][k] = number of distinct
   partitions among the first (k+1) compositions of integer j. */
static seq_t *prefix[MAX_J];
static unsigned char computed[MAX_J] = {0};

typedef struct {
    int j;                     /* integer whose compositions are generated */
    int pos;                   /* current depth in parts array */
    int parts[65];             /* current composition (parts) */
    int processed;             /* number of compositions processed so far */
    seq_t uniq;                /* number of distinct partitions seen so far */
    seq_t *prefix;             /* output array for j */
    seq_t *seen;               /* temporary storage of partition keys */
    int seen_cnt;              /* number of keys stored in seen */
} ctx_t;

/* Recursive generation of compositions of 'rem' in lexicographic order. */
static void rec(ctx_t *ctx, idx_t rem) {
    if (rem == 0) {
        /* copy parts into a temporary array */
        int tmp[65];
        for (int i = 0; i < ctx->pos; ++i) tmp[i] = ctx->parts[i];

        /* sort the parts descending (simple insertion sort) */
        for (int i = 1; i < ctx->pos; ++i) {
            int v = tmp[i];
            int s = i - 1;
            while (s >= 0 && tmp[s] < v) {
                tmp[s + 1] = tmp[s];
                --s;
            }
            tmp[s + 1] = v;
        }

        /* form a canonical key for the partition */
        seq_t key = 0;
        seq_t base = (seq_t)(ctx->j + 1);
        for (int i = 0; i < ctx->pos; ++i) {
            key = key * base + (seq_t)tmp[i];
        }

        /* check whether this partition has already been seen */
        int found = 0;
        for (int i = 0; i < ctx->seen_cnt; ++i) {
            if (ctx->seen[i] == key) {
                found = 1;
                break;
            }
        }
        if (!found) {
            ctx->seen[ctx->seen_cnt++] = key;
            ++ctx->uniq;
        }

        /* store the current count and advance */
        ctx->prefix[ctx->processed++] = ctx->uniq;
        return;
    }

    /* choose next part (1..rem) */
    for (int i = 1; i <= rem; ++i) {
        ctx->parts[ctx->pos++] = i;
        rec(ctx, rem - i);
        --ctx->pos;
    }
}

/* Precompute the prefix array for integer j */
static void precompute_j(int j) {
    idx_t total = (j == 1) ? 1 : ((idx_t)1 << (j - 1));

    /* allocate prefix array and temporary storage for partition keys */
    seq_t *arr  = (seq_t *)malloc((size_t)total * sizeof(seq_t));
    seq_t *seen = (seq_t *)malloc((size_t)total * sizeof(seq_t));

    ctx_t ctx;
    ctx.j          = j;
    ctx.pos        = 0;
    ctx.processed  = 0;
    ctx.uniq       = 0;
    ctx.prefix     = arr;
    ctx.seen       = seen;
    ctx.seen_cnt   = 0;

    rec(&ctx, j);

    free(seen);
    prefix[j] = arr;
    computed[j] = 1;
}

/* Main function required by the problem statement */
seq_t a(idx_t n) {
    if (n <= 0) return (seq_t)0;   /* should not occur for the defined sequence */

    /* Determine the minimal j with 2^(j‑1) ≥ n */
    int j = 1;
    idx_t pow = 1;
    while (pow < n) {
        ++j;
        pow <<= 1;
    }

    /* Compute prefix array for j on first use */
    if (!computed[j]) precompute_j(j);

    /* Return the number of distinct partitions after n compositions */
    return prefix[j][(size_t)(n - 1)];
}
