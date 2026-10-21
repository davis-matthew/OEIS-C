/*
 * A056472  Triangle read by rows in which row n lists all factorizations of
 *          n.
 * https://oeis.org/A056472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A056472
 *  Triangle read by rows: for each integer n≥1 output n itself, then all
 *  ordered factorizations of n into factors ≥2 in nondecreasing order.
 *  Numbers of each factorization are written in order, giving a single
 *  sequence a(n).  Thus a(1)=1, a(2)=2, a(3)=3, a(4)=4, a(5)=2, …
 *
 *  This implementation generates the sequence on demand, memoising
 *  already produced values.
 */

#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of the helper */
static void generate_row(uint64_t n);

/* ------------------------------------------------------------------ */
static seq_t *seq_buf       = NULL;   /* dynamic array of produced terms   */
static size_t seq_len      = 0;       /* current number of terms           */
static size_t seq_cap      = 0;       /* allocated capacity                */
static idx_t  next_row_n   = 1;       /* next n whose row is to be generated */

/* Push a value into the dynamic array, growing as needed */
static void push_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        seq_t *tmp = (seq_t*)realloc(seq_buf, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* no memory, terminate */
        seq_buf = tmp;
        seq_cap = new_cap;
    }
    seq_buf[seq_len++] = val;
}

/* Recursive depth‑first enumeration of nondecreasing factorizations */
static void dfs_factors(uint64_t rem, uint64_t start, seq_t *curr, int depth)
{
    for (uint64_t f = start; f <= rem / f; ++f) {
        if (rem % f != 0) continue;
        curr[depth] = (seq_t)f;
        uint64_t rest = rem / f;

        /* Output the factorization: curr[0..depth] followed by rest */
        for (int i = 0; i <= depth; ++i) push_seq(curr[i]);
        push_seq((seq_t)rest);

        /* Recurse further into rest, allowing subsequent factors ≥ f */
        dfs_factors(rest, f, curr, depth + 1);
    }
}

/* Generate all terms of the row corresponding to integer n */
static void generate_row(uint64_t n)
{
    push_seq((seq_t)n);     /* the number itself */

    if (n < 2) return;     /* no further factorizations */

    /* Temporary buffer for building factor chains */
    seq_t curr[64];         /* depth ≤ log2(n), 64 is ample for n≤10⁶ */
    dfs_factors(n, 2, curr, 0);
}

/* ------------------------------------------------------------------ */
/* Public function: returns the nth term of the sequence (1‑based) */
seq_t a(idx_t n)
{
    /* Generate rows until we have at least n terms */
    while (seq_len < n) {
        generate_row(next_row_n);
        ++next_row_n;
    }
    return seq_buf[n - 1];
}
