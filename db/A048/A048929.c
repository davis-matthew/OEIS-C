/*
 * A048929  Numbers that are the sum of 6 positive cubes in exactly 1 way.
 * https://oeis.org/A048929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 841, u32 n <= 841, u64 n <= 841, u128 n <= 841,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers that are the sum of six positive cubes in exactly one way. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Size limits – determined from the OEIS entry. */
#define MAX_CUBE_ROOT 26            /* 26^3 = 17576 <= 19417 < 27^3 */
#define MAX_SUM 19417

/* Count of representations for each sum up to MAX_SUM. 16 bits are ample. */
static uint16_t sum_count[MAX_SUM + 1];
/* Array of cubes for 1..MAX_CUBE_ROOT. */
static seq_t cube[MAX_CUBE_ROOT + 1];

/* Recursive generator of non‑decreasing 6‑tuples of cube indices. */
static void generate_rec(int depth, int start, seq_t sum)
{
    if (depth == 6) {
        if (sum <= (seq_t)MAX_SUM)
            sum_count[(idx_t)sum]++;    /* count this representation */
        return;
    }
    for (int v = start; v <= MAX_CUBE_ROOT; ++v) {
        seq_t new_sum = sum + cube[v];
        if (new_sum > (seq_t)MAX_SUM)   /* remaining cubes only increase the sum */
            break;
        generate_rec(depth + 1, v, new_sum);
    }
}

/* Build the sequence once – keeps a cache of the terms. */
static void build_sequence(seq_t **out_seq, idx_t *out_len)
{
    /* Fill cube array. */
    for (int i = 1; i <= MAX_CUBE_ROOT; ++i)
        cube[i] = (seq_t)i * i * i;

    /* Generate all non‑decreasing 6‑tuples and count sums. */
    generate_rec(0, 1, (seq_t)0);

    /* Count how many sums have exactly one representation. */
    idx_t cnt = 0;
    for (idx_t s = 1; s <= MAX_SUM; ++s)
        if (sum_count[s] == 1)
            ++cnt;

    /* Allocate storage and fill it. */
    seq_t *seq = (seq_t *)malloc(cnt * sizeof(seq_t));
    idx_t idx = 0;
    for (idx_t s = 1; s <= MAX_SUM; ++s)
        if (sum_count[s] == 1)
            seq[idx++] = (seq_t)s;

    *out_seq = seq;
    *out_len = cnt;
}

/* Public function – computes a(n) for the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static bool built = false;
    static seq_t *terms = NULL;
    static idx_t terms_len = 0;

    if (!built) {
        build_sequence(&terms, &terms_len);
        built = true;
    }

    /* Return 0 if n is out of range – the problem statement does not specify
       behaviour beyond the defined terms, but returning 0 is safer than UB. */
    if (n == 0 || n > terms_len)
        return (seq_t)0;

    return terms[n - 1];
}
