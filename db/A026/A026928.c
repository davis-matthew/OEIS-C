/*
 * A026928  Number of partitions of n into an even number of parts, the
 *          greatest being 4; also, a(n+7) = number of partitions of n+3 into
 *          an odd number of parts, each <=4.
 * https://oeis.org/A026928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an even number of parts,
 *            with each part ≤ 4 and at least one part equal to 4
 * Range:     u8 n <= 40, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/*  Static DP arrays for partitions with parts ≤ 3 (no 4) and with parts ≤ 4
    where at least one part is 4.  Two parity tables (even/odd parts) are
    kept for each case.  The arrays are expanded only when a larger value
    of n is requested and recomputed every time a larger n is encountered. */

static seq_t *e_no4 = NULL;      /* even‑part, no 4 */
static seq_t *o_no4 = NULL;      /* odd‑part, no 4 */
static seq_t *e_with4 = NULL;    /* even‑part, at least one 4 */
static seq_t *o_with4 = NULL;    /* odd‑part, at least one 4 */

static idx_t dp_len = 0;         /* N + 1 of the last computed N (exclusive) */
static idx_t dp_cap = 0;         /* allocated capacity (exclusive) */

/*  Resize the arrays to hold indices 0 … n.  If the capacity is increased,
    the newly allocated part is zeroed.  The function is called only when a
    larger N is required. */
static void ensure_size(idx_t n)
{
    /* Grow capacity to at least n+1, using powers of two for efficient growth */
    if (dp_cap <= n) {
        idx_t old_cap = dp_cap;
        if (dp_cap == 0) dp_cap = 1;
        while (dp_cap <= n) dp_cap <<= 1;

        e_no4 = (seq_t *)realloc(e_no4, dp_cap * sizeof(seq_t));
        o_no4 = (seq_t *)realloc(o_no4, dp_cap * sizeof(seq_t));
        e_with4 = (seq_t *)realloc(e_with4, dp_cap * sizeof(seq_t));
        o_with4 = (seq_t *)realloc(o_with4, dp_cap * sizeof(seq_t));
        if (!e_no4 || !o_no4 || !e_with4 || !o_with4) abort();

        memset(e_no4 + old_cap, 0, (dp_cap - old_cap) * sizeof(seq_t));
        memset(o_no4 + old_cap, 0, (dp_cap - old_cap) * sizeof(seq_t));
        memset(e_with4 + old_cap, 0, (dp_cap - old_cap) * sizeof(seq_t));
        memset(o_with4 + old_cap, 0, (dp_cap - old_cap) * sizeof(seq_t));
    }

    /* Zero all values up to n – we will recompute from scratch for this N */
    for (idx_t i = 0; i <= n; ++i) {
        e_no4[i] = o_no4[i] = e_with4[i] = o_with4[i] = 0;
    }
    e_no4[0] = (seq_t)1;              /* empty partition: even, no 4 */

    /* 1–3 : partitions with parts ≤ 3, parity toggled when adding a part */
    for (int coin = 1; coin <= 3; ++coin) {
        for (idx_t s = coin; s <= n; ++s) {
            seq_t ev = e_no4[s - coin];
            seq_t od = o_no4[s - coin];
            e_no4[s] += od;    /* adding coin flips parity */
            o_no4[s] += ev;
        }
    }

    /* 4 : now add the part 4.  Every addition of a 4 flips parity and
       guarantees the partition contains at least one 4.  Partitions that
       already contained a 4 are also updated here, allowing multiple 4s. */
    for (idx_t s = 4; s <= n; ++s) {
        seq_t ev_no4 = e_no4[s - 4];
        seq_t od_no4 = o_no4[s - 4];
        seq_t ev_w4   = e_with4[s - 4];
        seq_t od_w4   = o_with4[s - 4];

        e_with4[s] += od_no4;   /* from no‑4 partitions → now even */
        o_with4[s] += ev_no4;   /* from no‑4 partitions → now odd  */
        e_with4[s] += od_w4;    /* from existing with‑4 partitions → even */
        o_with4[s] += ev_w4;    /* from existing with‑4 partitions → odd  */
    }

    dp_len = n + 1;
}

/*  Required public function.  It may be called in any order; the internal
    DP tables are recomputed only when a larger value of n has been requested. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* not used, keeps semantics tidy */
    if (dp_len <= n)                 /* need to recompute tables up to n */
        ensure_size(n);
    return e_with4[n];              /* even‑part partitions with at least one 4 */
}
