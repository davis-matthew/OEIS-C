/*
 * A118315  a(1) = 1. a(2n) = smallest positive integer not occurring among
 *          the earlier terms of the sequence. a(2n+1) = the a(n)th positive
 *          integer among those positive integers not occurring earlier in the
 *          sequence.
 * https://oeis.org/A118315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1 a(2n) = smallest positive integer not occurring among
 *            a(1)…a(2n-1) a(2n+1) = the a(n)-th positive integer not
 *            occurring among a(1)…a(2n)
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* global state for memoisation */
static seq_t *seq = NULL;               /* a(1) stored at seq[0] */
static size_t seq_cap = 0;              /* allocated size of seq */
static size_t seq_len = 0;              /* number of terms already computed */

static bool *used = NULL;               /* used[i] == true if integer i has appeared */
static size_t used_cap = 0;             /* allocated size of used (indices 0 .. used_cap-1) */

static uint64_t mex = 1;                /* current smallest unused positive integer */

static void ensure_used_capacity(uint64_t v)
{
    if (v < used_cap)
        return;
    size_t new_cap = used_cap > 0 ? used_cap : 16;
    while (new_cap <= v)
        new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_used)
        abort();
    for (size_t i = used_cap; i < new_cap; ++i)
        new_used[i] = false;
    used = new_used;
    used_cap = new_cap;
}

static void advance_mex(void)
{
    uint64_t m = mex;
    ensure_used_capacity(m);
    while (used[m]) {
        ++m;
        ensure_used_capacity(m);
    }
    mex = m;
}

static void ensure_seq_capacity(idx_t n)          /* compute terms up to index n (1‑based) */
{
    while (seq_len < (size_t)n) {
        idx_t i = seq_len + 1;          /* next 1‑based index to compute */
        seq_t val;

        if (i == 1) {
            val = (seq_t)1;
            /* a(1) is 1 – register it as used */
            ensure_used_capacity(1);
            used[1] = true;
            advance_mex();
        } else if ((i & 1) == 0) {        /* even position: a(2n) */
            val = (seq_t)mex;
            uint64_t v = (uint64_t)val;
            ensure_used_capacity(v);
            used[v] = true;
            advance_mex();
        } else {                          /* odd position: a(2n+1) */
            idx_t m = (i - 1) / 2;
            if ((size_t)m > seq_len)
                ensure_seq_capacity(m);   /* guarantee a(m) exists */
            idx_t k = (idx_t)seq[m - 1];   /* a(m) */
            idx_t count = 0;
            uint64_t candidate = 1;
            while (true) {
                ensure_used_capacity(candidate);
                if (!used[candidate]) {
                    ++count;
                    if (count == k)
                        break;
                }
                ++candidate;
            }
            val = (seq_t)candidate;
            ensure_used_capacity(candidate);
            used[candidate] = true;
            if (candidate == mex)
                advance_mex();
        }

        /* grow seq array if needed */
        if ((size_t)i > seq_cap) {
            size_t new_cap = seq_cap > 0 ? seq_cap << 1 : 16;
            while (new_cap < (size_t)i)
                new_cap <<= 1;
            seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            if (!new_seq)
                abort();
            seq = new_seq;
            seq_cap = new_cap;
        }

        seq[seq_len] = val;
        ++seq_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined case – return 0 for safety */
    ensure_seq_capacity(n);
    return seq[n - 1];
}
