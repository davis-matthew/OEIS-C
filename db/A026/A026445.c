/*
 * A026445  a(n) = least positive integer > a(n-1) and not a(i)*a(j)+2 for
 *          1<=i<j<=n, with initial terms 2,3.
 * https://oeis.org/A026445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   hash_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026445: a(n) = least positive integer > a(n-1) that is not of
   the form a(i)*a(j)+2 for any 1 ≤ i < j < n.  Initial values 2, 3. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/hash_seq.h"

/* ---- Hash set for previous terms --------------------------------------- */
static seq_t *hashset = NULL;
static size_t set_cap = 0;
static size_t set_len = 0;

static void set_rehash(void)
{
    size_t old_cap = set_cap;
    seq_t *old_set = hashset;
    set_cap = (old_cap > 0) ? old_cap << 1 : 8;
    hashset = (seq_t *)calloc(set_cap, sizeof(seq_t));
    set_len = 0;
    for (size_t i = 0; i < old_cap; ++i) {
        if (old_set[i]) {
            size_t h = hash_seq(&old_set[i]) & (set_cap - 1);
            while (hashset[h]) h = (h + 1) & (set_cap - 1);
            hashset[h] = old_set[i];
            ++set_len;
        }
    }
    free(old_set);
}

static bool contains_seq(const seq_t *x)
{
    if (!hashset) return false;
    size_t h = hash_seq(x) & (set_cap - 1);
    while (hashset[h]) {
        if (hashset[h] == *x) return true;
        h = (h + 1) & (set_cap - 1);
    }
    return false;
}

static void add_seq_set(const seq_t *x)
{
    if (!hashset) {
        set_cap = 8;
        hashset = (seq_t *)calloc(set_cap, sizeof(seq_t));
    }
    if (set_len * 2 >= set_cap) set_rehash();
    size_t h = hash_seq(x) & (set_cap - 1);
    while (hashset[h]) h = (h + 1) & (set_cap - 1);
    hashset[h] = *x;
    ++set_len;
}

/* ---- Growing array for the sequence ------------------------------------- */
static seq_t *seq = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static void seq_append(const seq_t *val)
{
    if (seq_len == seq_cap) {
        seq_cap = (seq_cap > 0) ? seq_cap << 1 : 8;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = *val;
    add_seq_set(val);
}

/* ---- Initialization ----------------------------------------------------- */
static bool initialized = false;

static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* initial terms 2, 3 */
    seq_cap = 8;
    seq = (seq_t *)calloc(seq_cap, sizeof(seq_t));
    seq_len = 2;
    seq[0] = (seq_t)2;
    seq[1] = (seq_t)3;
    add_seq_set(&seq[0]);
    add_seq_set(&seq[1]);
}

/* ---- Main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init();

    while (seq_len < n) {
        seq_t prev = seq[seq_len - 1];
        seq_t candidate = prev + (seq_t)1;

        for (;;) {
            seq_t m = candidate - (seq_t)2;
            bool forbidden = false;

            for (idx_t d = 1; ; ++d) {
                seq_t dseq = (seq_t)d;
                seq_t div = m / dseq;
                if (div < dseq) break;            /* reached sqrt(m) */
                if (m % dseq == (seq_t)0) {
                    seq_t e = div;
                    if (e > dseq &&
                        contains_seq(&dseq) &&
                        contains_seq(&e)) {
                        forbidden = true;
                        break;
                    }
                }
            }
            if (!forbidden) break;      /* found minimal candidate   */
            ++candidate;
        }
        seq_append(&candidate);
    }
    return seq[n - 1];
}
