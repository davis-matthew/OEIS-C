/*
 * A138151  Irregular triangle read by rows in which rows 1..n (when read
 *          together) list all the parts in the partitions of n and row n
 *          starts with the partitions of n that do not contain 1 as a part
 *          (in the order used for A080577).
 * https://oeis.org/A138151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 4630, u16 n <= 4630, u32 n <= 4630, u64 n <= 4630,
 *            u128 n <= 4630, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A138151.
   A(n) is determined by flattening an irregular triangle whose
   row n consists of all parts of all partitions of n that do not
   contain a part 1 (in reverse‑lexicographic order), followed by
   p(n-1) ones, where p(m) is the partition number.
   The function lazily generates the sequence up to the requested
   index and caches it for subsequent calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* maximum row we will pre‑compute; enough for any index up to 100 000.
   Partition numbers up to this row fit in 64‑bit. */
#define MAX_ROW 200

/* permanent cache of the sequence terms */
static seq_t *seq_cache = NULL;
static idx_t   seq_len  = 0;
static idx_t   seq_cap  = 0;

/* partition numbers p[0..MAX_ROW] (64‑bit) */
static idx_t p_counts[MAX_ROW + 1] = {0};

/* current last row that has been generated */
static idx_t current_row = 0;

/* ------------------------------------------------------------------ */
/* Helper: ensure the sequence cache has room for at least [need] elems */
static void _ensure_seq_cap(idx_t need)
{
    if (need <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In an environment where memory allocation fails we abort the
           program (the harness expects the function to return a valid
           sequence for all requested indices). */
        abort();
    }
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/* Helper: append one value to the sequence cache */
static void _push_seq(seq_t val)
{
    _ensure_seq_cap(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Compute partition numbers p[0..MAX_ROW] once. */
static void _init_partition_counts(void)
{
    for (idx_t i = 0; i <= MAX_ROW; ++i)
        p_counts[i] = 0;
    p_counts[0] = 1;
    for (idx_t i = 1; i <= MAX_ROW; ++i) {
        for (idx_t j = i; j <= MAX_ROW; ++j)
            p_counts[j] += p_counts[j - i];
    }
}

/* ------------------------------------------------------------------ */
/* Recursive generation of partitions of 'rem' with parts >=2 and
   non‑increasing (<=maxp). For each complete partition, each part
   is appended to the global sequence cache. */
static void _gen_parts_and_push(idx_t rem, idx_t maxp, int *stack, int depth)
{
    if (rem == 0) {
        for (int i = 0; i < depth; ++i)
            _push_seq((seq_t)stack[i]);
        return;
    }
    for (idx_t part = (maxp < rem ? maxp : rem); part >= 2; --part) {
        stack[depth] = (int)part;
        _gen_parts_and_push(rem - part, part, stack, depth + 1);
    }
}

/* Generate row 'n' of the triangle and append its terms to the cache. */
static void _generate_row(idx_t n)
{
    int stack[MAX_ROW]; /* enough for any row <= MAX_ROW */
    _gen_parts_and_push(n, n, stack, 0);
    /* append p(n-1) ones */
    idx_t ones = p_counts[n - 1];
    for (idx_t i = 0; i < ones; ++i)
        _push_seq((seq_t)1);
}

/* ------------------------------------------------------------------ */
/* Extend the cache until it contains at least 'n' terms. */
static void _ensure_cache(idx_t n)
{
    if (n <= seq_len)
        return;
    if (current_row == 0) {
        /* first time: initialise partition counts */
        _init_partition_counts();
    }
    while (seq_len < n) {
        ++current_row;
        if (current_row > MAX_ROW) {
            /* should never happen for the requested index range */
            abort();
        }
        _generate_row(current_row);
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* sequence is 1‑based; n>=1 by spec */
    _ensure_cache(n);
    return seq_cache[n - 1];
}
