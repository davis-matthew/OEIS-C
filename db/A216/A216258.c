/*
 * A216258  Numbers n such that 4n is a partition number.
 * https://oeis.org/A216258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p(m) / 4) where p(m) is the m‑th partition number and
 *            p(m) is divisible by 4. The sequence is enumerated in increasing
 *            order of m; a(1) is the first such quotient
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the partition numbers via the usual
   generalized pentagonal number recurrence.  Partition values are
   stored in a __int128 array (large enough for all values relevant to
   64‑bit seq_t).  The result of each division that is divisible by 4
   is stored in a dynamic array of seq_t.  Subsequent calls to a(n)
   simply retrieve the *n*‑th stored value.  The algorithm is
   <O(n) per call. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

static __int128 *parts = NULL;      /* p(0)..p(k) */
static size_t  parts_len = 0;       /* number of computed partitions */
static size_t  parts_cap = 0;       /* allocated capacity for parts */

static seq_t *seq_vals = NULL;      /* stored a(1)..a(k) */
static size_t seq_len = 0;          /* number of stored a values */
static size_t seq_cap = 0;          /* allocated capacity for seq_vals */

static void ensure_parts_cap(size_t new_cap)
{
    if (new_cap <= parts_cap) return;
    size_t cap = parts_cap == 0 ? 16 : parts_cap;
    while (cap < new_cap) cap <<= 1;
    __int128 *tmp = (__int128*)realloc(parts, cap * sizeof(__int128));
    if (!tmp) abort();
    parts = tmp;
    parts_cap = cap;
}

static void ensure_seq_cap(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap == 0 ? 8 : seq_cap;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_vals, cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_vals = tmp;
    seq_cap = cap;
}

static void push_seq_val(seq_t val)
{
    ensure_seq_cap(seq_len + 1);
    seq_vals[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 */

    /* first call initialises buffers and p(0)=1 */
    if (parts_len == 0) {
        ensure_parts_cap(1);
        parts[0] = 1;
        parts_len = 1;
    }

    /* generate new partition numbers until we have enough a(n) values */
    while (seq_len < (size_t)n) {
        int k = (int)parts_len;           /* index of new partition p(k) */

        /* compute p(k) from the recurrence */
        __int128 sum = 0;
        for (int m = 1; ; ++m) {
            int g1 = m * (3 * m - 1) / 2;
            if (g1 > k) break;
            sum += (m & 1) ? parts[k - g1] : -parts[k - g1];
            int g2 = m * (3 * m + 1) / 2;
            if (g2 > k) continue;
            sum += (m & 1) ? parts[k - g2] : -parts[k - g2];
        }

        /* store the partition number */
        ensure_parts_cap(parts_len + 1);
        parts[parts_len++] = sum;

        /* if divisible by 4, store the quotient as a sequence value */
        if ((seq_t)(sum % 4) == 0) {
            push_seq_val((seq_t)(sum / 4));
        }
    }

    return seq_vals[n - 1];
}
