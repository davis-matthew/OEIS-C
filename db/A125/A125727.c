/*
 * A125727  a(1)=1. a(2)=2. a(n) = the smallest positive integer not occurring
 *          earlier in the sequence such that a(n-2)+a(n-1) is congruent to
 *          a(n) (mod n).
 * https://oeis.org/A125727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest positive integer not occurring earlier such
 *            that a(n-2) + a(n-1) ≡ a(n) (mod n), with a(1) = 1, a(2) = 2.
 *            The sequence is memoised so that calls may arrive in any order
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Internal storage for the sequence and used values. */
static seq_t *seq      = NULL;   /* sequence values (0‑based) */
static size_t seq_cap  = 0;      /* allocated capacity of seq      */
static size_t seq_len  = 0;      /* number of terms computed      */

static seq_t *seen     = NULL;   /* array of values that have appeared */
static size_t seen_cap = 0;      /* allocated capacity of seen      */
static size_t seen_len = 0;      /* number of seen values           */

static seq_t min_unused = (seq_t)1;   /* smallest positive integer not yet used */

/* Ensure the sequence array can hold at least 'need' elements. */
static void ensure_seq_cap(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Ensure the seen array can hold at least 'need' elements. */
static void ensure_seen_cap(size_t need)
{
    if (seen_cap < need) {
        size_t new_cap = seen_cap ? seen_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seen = (seq_t *)realloc(seen, new_cap * sizeof(seq_t));
        seen_cap = new_cap;
    }
}

/* Linear search for 'v' in the seen array. */
static bool seen_value(seq_t v)
{
    for (size_t i = 0; i < seen_len; ++i)
        if (seen[i] == v) return true;
    return false;
}

/* Add 'v' to the seen array. */
static void mark_seen(seq_t v)
{
    ensure_seen_cap(seen_len + 1);
    seen[seen_len++] = v;
}

/* Advance min_unused until it refers to an unseen value. */
static void update_min_unused(void)
{
    while (seen_value(min_unused)) ++min_unused;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    /* Compute terms up to index n if not already done. */
    while (seq_len < (size_t)n) {
        seq_t val;
        idx_t m = seq_len + 1;               /* index of the term to compute (1‑based) */

        if (m == 1) {
            val = (seq_t)1;
        } else if (m == 2) {
            val = (seq_t)2;
        } else {
            /* Remainder required for the congruence a(n-2)+a(n-1) ≡ a(n) (mod n). */
            seq_t rem = (seq[m-3] + seq[m-2]) % (seq_t)m;
            seq_t mod = (seq_t)m;

            /* Find the smallest integer >= min_unused that satisfies the modulus. */
            seq_t cand;
            if (min_unused <= rem) {
                cand = rem;
            } else {
                seq_t delta = min_unused - rem;
                seq_t q = delta / mod;
                if (delta % mod != 0) ++q;
                cand = rem + q * mod;
            }

            /* Advance by 'mod' until we hit an unused value. */
            while (seen_value(cand))
                cand += mod;

            val = cand;
        }

        /* Store the new term and update bookkeeping. */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = val;
        mark_seen(val);
        if (val == min_unused) update_min_unused();
    }

    return seq[n - 1];
}
