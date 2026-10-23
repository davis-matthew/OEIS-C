/*
 * A045142  Numbers whose base-4 representation contains exactly one 2 and
 *          four 3's.
 * https://oeis.org/A045142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045142: numbers whose base‑4 representation contains exactly one 2
   and exactly four 3's.  The values are generated in increasing order. */

#include <stddef.h>
#include <stdlib.h>

/* Standard comparator for qsort.  Works for both integer and
   arbitrary‑precision seq_t types that provide operator<. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : ((*b < *a) ? 1 : 0);
}

/* Max number of terms we will store – sufficient for n ≤ 10000. */
#define MAX_CACHE 20000

static seq_t cache[MAX_CACHE];   /* stored terms, ascending order */
static size_t cache_len = 0;     /* number of terms stored */

/* Generate all numbers of length L (L ≥ 5) whose base‑4 representation
   contains exactly one digit 2 and four digits 3, and whose most
   significant digit is non‑zero.  The generated numbers are appended
   to the global cache in ascending order. */
static void generate_length(idx_t L)
{
    /* pre‑compute powers of 4 up to L‑1 */
    seq_t pow4[64];
    pow4[0] = (seq_t)1;
    for (idx_t i = 1; i < L; ++i)
        pow4[i] = pow4[i-1] * (seq_t)4;

    /* temporary buffer for numbers of this length */
    seq_t local[MAX_CACHE];
    size_t local_len = 0;

    /* choose the position of the single digit 2 */
    for (idx_t pos2 = 0; pos2 < L; ++pos2) {
        /* choose four distinct positions for the digit 3 among the
           remaining positions.  Nested loops enumerate all
           combinations of indices a < b < c < d.  */
        for (idx_t a = 0; a < L; ++a) {
            if (a == pos2) continue;
            for (idx_t b = a + 1; b < L; ++b) {
                if (b == pos2) continue;
                for (idx_t c = b + 1; c < L; ++c) {
                    if (c == pos2) continue;
                    for (idx_t d = c + 1; d < L; ++d) {
                        if (d == pos2) continue;
                        idx_t pos3[4] = {a, b, c, d};

                        /* base value fixed by the 2 and the 3 digits */
                        seq_t val = (seq_t)2 * pow4[pos2];
                        val += (seq_t)3 * pow4[a];
                        val += (seq_t)3 * pow4[b];
                        val += (seq_t)3 * pow4[c];
                        val += (seq_t)3 * pow4[d];

                        /* collect positions that are not yet fixed */
                        idx_t free_pos[64];
                        size_t free_cnt = 0;
                        for (idx_t pos = 0; pos < L; ++pos) {
                            if (pos == pos2 ||
                                pos == a || pos == b || pos == c || pos == d)
                                continue;
                            free_pos[free_cnt++] = pos;
                        }

                        /* for each subset of the free positions, put a
                           digit 1 or 0.  The subset is represented by
                           a bit mask. */
                        size_t mask_limit = (size_t)1 << free_cnt;
                        for (size_t mask = 0; mask < mask_limit; ++mask) {
                            seq_t x = val;
                            for (size_t k = 0; k < free_cnt; ++k)
                                if (mask & ((size_t)1 << k))
                                    x += pow4[free_pos[k]];

                            /* skip numbers with a leading zero */
                            seq_t msd = x / pow4[L - 1];
                            if (msd == (seq_t)0) continue;

                            local[local_len++] = x;
                        }
                    }
                }
            }
        }
    }

    /* sort the numbers of this length and append them to the global cache */
    qsort(local, local_len, sizeof(seq_t), cmp_seq);
    for (size_t i = 0; i < local_len && cache_len < MAX_CACHE; ++i)
        cache[cache_len++] = local[i];
}

/* Ensure that the cache contains at least n terms (n ≥ 1).  This
   function expands the cache by generating longer lengths until
   cache_len ≥ n. */
static void ensure_cache(idx_t n)
{
    static idx_t last_len = 4;   /* highest length processed so far */
    while (cache_len < n) {
        ++last_len;              /* process the next length */
        generate_length(last_len);
    }
}

/* Public function: a(n) returns the n‑th term (n ≥ 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n = 0, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
