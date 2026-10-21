/*
 * A080728  a(0) = 3; for n>0, a(n) is taken to be the smallest positive
 *          integer greater than a(n-1) which is consistent with the condition
 *          "n is a member of the sequence if and only if a(n) == 2 mod 3".
 * https://oeis.org/A080728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080728
 * a(0) = 3
 * For n > 0, a(n) is the smallest positive integer greater than a(n-1)
 * that satisfies:
 *     n appears somewhere in the range of a   ⇔   a(n) ≡ 2 (mod 3)
 * This definition is equivalent to the functional equation a(a(n)) = 3*n+8.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* --- static cache ------------------------------------------------- */
static seq_t *seq      = NULL;   /* computed values of a(n) */
static idx_t seq_cap   = 0;      /* allocated capacity of seq   */
static idx_t seq_len   = 0;      /* number of elements stored   */

static bool *seen     = NULL;    /* seen[v] == true  ⇔  v has appeared as a term */
static idx_t seen_cap = 0;       /* allocated capacity of seen */

/* enlarge seq array if needed */
static void ensure_seq_cap(idx_t cap)
{
    if (seq_cap >= cap) return;
    idx_t new_cap = seq_cap == 0 ? 4 : seq_cap;
    while (new_cap < cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* enlarge seen array if needed (new entries are zeroed) */
static void ensure_seen_cap(idx_t cap)
{
    if (seen_cap >= cap) return;
    idx_t new_cap = seen_cap == 0 ? 4 : seen_cap;
    while (new_cap < cap) new_cap <<= 1;
    size_t old_cap = seen_cap;
    bool *tmp = (bool *)realloc(seen, new_cap * sizeof(bool));
    if (!tmp) abort();
    if (old_cap < new_cap)
        memset(tmp + old_cap, 0, (new_cap - old_cap) * sizeof(bool));
    seen = tmp;
    seen_cap = new_cap;
}

/* mark a term as seen */
static void mark_seen(seq_t val)
{
    size_t idx = (size_t)val;
    if (idx >= seen_cap) ensure_seen_cap(idx + 1);
    seen[idx] = true;
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (seq == NULL) { ensure_seq_cap(8); seq_len = 0; }
    if (seen == NULL) { ensure_seen_cap(8); }

    /* shrink evidence: n may be larger than seen array capacity */
    if (n >= seen_cap) ensure_seen_cap(n + 1);

    /* base term a(0) = 3 */
    if (seq_len == 0) {
        seq[0] = (seq_t)3;
        seq_len = 1;
        mark_seen((seq_t)3);
    }

    /* extend cache up to requested index */
    if (seq_len <= n) {
        ensure_seq_cap(n + 1);
        for (idx_t i = seq_len; i <= n; ++i) {
            if (i == 0) continue;   /* already handled */

            bool need_two = (i < seen_cap) ? seen[i] : false; /* membership of i */

            seq_t m = seq[i - 1] + (seq_t)1;
            while (true) {
                seq_t rem = m % (seq_t)3;
                if ((need_two && rem == (seq_t)2) ||
                    (!need_two && rem != (seq_t)2))
                    break;
                m += (seq_t)1;
            }
            seq[i] = m;
            mark_seen(m);
        }
        seq_len = n + 1;
    }

    return seq[n];
}
