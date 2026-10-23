/*
 * A022948  Duplicate of A022443.
 * https://oeis.org/A022948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = a(n-1) + c(n), where c(n) is the n‑th smallest * positive
 *            integer not in the sequence. Initial values: a(1)=1, * a(2)=4
 * Range:     i8 n <= 13, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compile with C11 or C++17.  seq_t and idx_t are supplied by the
 * build system. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static state for incremental computation. */
static idx_t   seq_len     = 0;      /* number of terms computed so far */
static seq_t  *seq_arr     = NULL;   /* array of sequence values (0‑based) */
static idx_t   seq_cap     = 0;      /* capacity of seq_arr */

static uint64_t *used_bits = NULL;  /* bitset of used numbers (indexed by uint64_t) */
static idx_t   used_blocks = 0;     /* number of 64‑bit blocks in used_bits */

static idx_t   scan_pos      = 0;    /* current position in natural numbers */
static idx_t   missing_cnt   = 0;    /* number of missing numbers seen so far */

/* Ensure used_bits has room for index `idx` (1‑based).  */
static void ensure_used_capacity(idx_t idx)
{
    /* Compute the block index needed for `idx`. */
    idx_t needed = (idx >> 6) + 1;            /* number of 64‑bit blocks needed */
    if (needed <= used_blocks) return;

    idx_t new_blocks = used_blocks ? used_blocks << 1 : 64;
    if (new_blocks < needed) new_blocks = needed;

    uint64_t *tmp = (uint64_t *)realloc(used_bits, new_blocks * sizeof(uint64_t));
    if (!tmp) exit(1);                       /* allocation failure */
    for (idx_t i = used_blocks; i < new_blocks; ++i) tmp[i] = 0;
    used_bits   = tmp;
    used_blocks = new_blocks;
}

/* Test whether number `pos` is already used.  */
static bool used_bit(idx_t pos)
{
    idx_t block = pos >> 6;
    return (used_bits[block] >> (pos & 63)) & 1ULL;
}

/* Mark number `pos` as used.  */
static void set_used_bit(idx_t pos)
{
    idx_t block = pos >> 6;
    used_bits[block] |= 1ULL << (pos & 63);
}

/* Ensure seq_arr can hold at least `n` elements. */
static void ensure_seq_capacity(idx_t n)
{
    if (seq_cap >= n) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < n) new_cap = n;
    seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_arr = tmp;
    seq_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, just in case */

    /* Initialise structures on first call. */
    if (!seq_arr) {
        seq_cap = n > 16 ? n : 16;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) exit(1);
    }
    ensure_seq_capacity(n);

    /* Seed the first two terms if necessary. */
    if (seq_len < 2) {
        seq_arr[0] = (seq_t)1;
        seq_arr[1] = (seq_t)4;
        ensure_used_capacity(4);
        set_used_bit(1);
        set_used_bit(4);
        seq_len = 2;
    }

    /* Produce terms up to the requested index. */
    while (seq_len < n) {
        idx_t next_idx = seq_len + 1;   /* target n to compute (1‑based) */

        /* Find the next missing integer. */
        while (missing_cnt < next_idx) {
            ++scan_pos;
            ensure_used_capacity(scan_pos);
            if (!used_bit(scan_pos)) ++missing_cnt;
        }
        seq_t c_val = (seq_t)scan_pos;   /* the n‑th missing number */

        /* Compute the new sequence term. */
        seq_t val   = seq_arr[seq_len - 1] + c_val;
        seq_arr[seq_len] = val;

        /* Mark the new term as used. */
        ensure_used_capacity((idx_t)val);
        set_used_bit((idx_t)val);

        ++seq_len;
    }
    return seq_arr[n - 1];
}
