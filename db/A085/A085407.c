/*
 * A085407  Runs of zeros in binomial(3k+2,k+1)/(3k+2) modulo 2 (A085405).
 * https://oeis.org/A085407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A085407 (runs of zeros in binomial(3k+2,k+1)/(3k+2) modulo 2)
 *
 * Construction:  start with strings
 *   S(1) = {1},
 *   S(2) = {1,3},
 *   S(3) = {1,5};
 * for k > 3, concatenate all previous strings except S(k‑1),
 * then replace the last element L by 4·L – (–1)^k.
 *
 * The global sequence is the concatenation of all S(k).
 *
 * Implementation notes:
 *   * The sequence is built incrementally until it contains the requested index.
 *   * All copies are performed element‑wise to remain valid when seq_t may be a
 *     non‑trivial type (e.g. an arbitrary‑precision integer class).
 *   * No non‑portable extensions, only C11/ISO C++17 constructs.
 *
 * This algorithm is O(n) in the size of the output produced so far, which
 * satisfies the time requirement for n up to ~10⁴.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global storage for the concatenated sequence and block lengths. */
static seq_t *seq      = NULL;   /* concatenation of all S(k) seen so far */
static size_t seq_len  = 0;      /* number of elements stored in seq   */
static size_t seq_cap  = 0;      /* capacity of seq                    */

static idx_t *len_arr  = NULL;   /* length of each block S(k)          */
static size_t len_cnt  = 0;      /* number of blocks already generated */
static size_t len_cap  = 0;      /* capacity of len_arr                */

/* Ensure seq has capacity for at least `n` additional elements. */
static void ensure_seq_capacity(size_t n)
{
    if (seq_len + n > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 64;
        while (seq_len + n > new_cap) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure len_arr has capacity for at least `n` additional entries. */
static void ensure_len_capacity(size_t n)
{
    if (len_cnt + n > len_cap) {
        size_t new_cap = len_cap ? len_cap << 1 : 32;
        while (len_cnt + n > new_cap) new_cap <<= 1;
        len_arr = (idx_t *)realloc(len_arr, new_cap * sizeof(idx_t));
        len_cap = new_cap;
    }
}

/* Append `cnt` elements starting at `src` to the global sequence. */
static void append_seq(const seq_t *src, size_t cnt)
{
    ensure_seq_capacity(cnt);
    for (size_t i = 0; i < cnt; ++i)
        seq[seq_len + i] = src[i];
    seq_len += cnt;
}

/* Public sequence generator as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Build up the sequence until it contains at least n terms. */
    while (seq_len < (size_t)n) {
        /* Determine next block index k (1‑based). */
        idx_t k = (idx_t)(len_cnt + 1);

        /* Allocate room for the length of the new block. */
        ensure_len_capacity(1);

        if (k == 1) {
            seq_t v[1] = { (seq_t)1 };
            append_seq(v, 1);
            len_arr[len_cnt++] = 1;
        }
        else if (k == 2) {
            seq_t v[2] = { (seq_t)1, (seq_t)3 };
            append_seq(v, 2);
            len_arr[len_cnt++] = 2;
        }
        else if (k == 3) {
            seq_t v[2] = { (seq_t)1, (seq_t)5 };
            append_seq(v, 2);
            len_arr[len_cnt++] = 2;
        }
        else {
            /* Length of the previous block S(k‑1). */
            idx_t prev_len = len_arr[len_cnt - 1];

            /* Build the new block by copying the prefix that excludes S(k‑1). */
            size_t new_block_len = seq_len - (size_t)prev_len;  /* sum of lengths S(1)…S(k‑2) */
            seq_t *new_block = (seq_t *)malloc(new_block_len * sizeof(seq_t));
            if (!new_block) exit(1);  /* allocation failure */

            for (size_t i = 0; i < new_block_len; ++i)
                new_block[i] = seq[i];

            /* Replace the last element L. */
            seq_t L = new_block[new_block_len - 1];
            seq_t new_last;
            if ((k & 1U) == 0)          /* k even: 4·L – 1 */
                new_last = (4 * L) - 1;
            else                         /* k odd : 4·L + 1 */
                new_last = (4 * L) + 1;
            new_block[new_block_len - 1] = new_last;

            /* Append the new block to the global sequence. */
            append_seq(new_block, new_block_len);
            free(new_block);

            /* Record the length of this block. */
            len_arr[len_cnt++] = (idx_t)new_block_len;
        }
    }

    /* Return the element at the requested 1‑based index. */
    return seq[(size_t)n - 1];
}
