/*
 * A230374  The numbers n such that during dividing n by all positive integers
 *          not exceeding n the remainder 0 occurs most often.
 * https://oeis.org/A230374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for n such that the number of
   zero remainders in 1..n is strictly larger than that for all other
   remainders. */

#include <stdlib.h>
#include <string.h>

/* static cache of sequence values (1‑based index) */
static seq_t *seq = NULL;
static size_t seq_len   = 0;   /* number of terms cached */
static size_t seq_cap   = 0;   /* allocated capacity */

/* static cache for remainder counts */
static unsigned int *cnt_buf = NULL;
static size_t cnt_cap   = 0;   /* allocated capacity for counts */

/* last natural number examined */
static idx_t last_checked = 0;

/* ensure seq has room for at least `need` elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* ensure count buffer can hold remainders up to `n-1` */
static void ensure_cnt_capacity(size_t n)
{
    if (n <= cnt_cap) return;
    size_t new_cap = cnt_cap ? cnt_cap : 16;
    while (new_cap < n) new_cap <<= 1;
    cnt_buf = (unsigned int *)realloc(cnt_buf, new_cap * sizeof(unsigned int));
    cnt_cap = new_cap;
}

/* does the number `x` have the most frequent remainder 0? */
static int qualifies(idx_t x)
{
    size_t m = (size_t)x;      /* natural number being tested */
    ensure_cnt_capacity(m);
    unsigned int *cnt = cnt_buf;
    memset(cnt, 0, m * sizeof(unsigned int));

    for (size_t d = 1; d <= m; ++d) {
        size_t r = (size_t)(x % (idx_t)d);
        ++cnt[r];
    }

    unsigned int zeroCount  = cnt[0];
    unsigned int maxOther   = 0;
    for (size_t r = 1; r < m; ++r) {
        if (cnt[r] > maxOther) maxOther = cnt[r];
    }
    return zeroCount > maxOther;
}

/* OEIS sequence entry function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; requests <1 are undefined */

    size_t target = (size_t)n;
    while (seq_len < target) {
        ++last_checked;
        if (qualifies(last_checked)) {
            ensure_seq_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq[n - 1];
}
