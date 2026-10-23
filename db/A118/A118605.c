/*
 * A118605  a(0)=0, a(1)=1; and for n>1: if n in binary representation is
 *          contained in the concatenation of all preceding terms then a(n)=1
 *          else a(n)=0.
 * https://oeis.org/A118605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the binary representation of n occurs as a substring
 *            in the concatenation of all previous a(k), else 0 (a(0)=0,
 *            a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- state for suffix automaton ---------- */
typedef struct {
    int link;           /* suffix link */
    int next[2];        /* transitions for '0' and '1' */
    int len;            /* length of longest string in this state */
} sa_state_t;

static sa_state_t *sa = NULL;    /* array of states */
static size_t sa_cap = 0;        /* capacity of sa[] */
static size_t sa_size = 0;       /* number of states used */
static int sa_last = 0;          /* last state added */

/* ensure capacity for new states */
static void sa_ensure(size_t new_cap)
{
    if (new_cap <= sa_cap) return;
    size_t cap = sa_cap ? sa_cap : 4;
    while (cap < new_cap) cap <<= 1;
    sa = (sa_state_t *)realloc(sa, cap * sizeof(sa_state_t));
    sa_cap = cap;
}

/* initialise automaton (empty string) */
static void sa_init(void)
{
    sa_ensure(2);
    sa[0].len   = 0;
    sa[0].link  = -1;
    sa[0].next[0] = -1;
    sa[0].next[1] = -1;
    sa_size = 1;
    sa_last = 0;
}

/* extend automaton by character c ('0' or '1') */
static void sa_extend(int c)
{
    sa_ensure(sa_size + 1);
    int cur = (int)sa_size++;
    sa[cur].len = sa[sa_last].len + 1;
    sa[cur].next[0] = sa[cur].next[1] = -1;
    int p = sa_last;
    while (p != -1 && sa[p].next[c] == -1) {
        sa[p].next[c] = cur;
        p = sa[p].link;
    }
    if (p == -1) {
        sa[cur].link = 0;
    } else {
        int q = sa[p].next[c];
        if (sa[p].len + 1 == sa[q].len) {
            sa[cur].link = q;
        } else {
            sa_ensure(sa_size + 1);
            int clone = (int)sa_size++;
            sa[clone].len   = sa[p].len + 1;
            sa[clone].next[0] = sa[q].next[0];
            sa[clone].next[1] = sa[q].next[1];
            sa[clone].link  = sa[q].link;
            while (p != -1 && sa[p].next[c] == q) {
                sa[p].next[c] = clone;
                p = sa[p].link;
            }
            sa[q].link = sa[cur].link = clone;
        }
    }
    sa_last = cur;
}

/* check whether pattern (binary string) exists in automaton */
static bool pat_exists(const char *pat, size_t len)
{
    int cur = 0;
    for (size_t i = 0; i < len; ++i) {
        int bit = (pat[i] == '1') ? 1 : 0;
        cur = sa[cur].next[bit];
        if (cur == -1) return false;
    }
    return true;
}

/* ---------- sequence storage ---------- */
static seq_t *seq = NULL;      /* array of sequence values */
static size_t seq_cap = 0;     /* allocated capacity */
static size_t seq_len = 0;     /* number of values stored */

static void seq_ensure(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    size_t cap = seq_cap ? seq_cap : 4;
    while (cap < new_cap) cap <<= 1;
    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    seq_cap = cap;
}

/* compute binary representation of n into buf (null‑terminated). */
static void bin_str(idx_t n, char *buf)
{
    if (n == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    int pos = 0;
    idx_t tmp = n;
    while (tmp) {
        buf[pos++] = (tmp & 1) ? '1' : '0';
        tmp >>= 1;
    }
    /* reverse */
    for (int i = 0; i < pos / 2; ++i) {
        char t = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = t;
    }
    buf[pos] = '\0';
}

/* build terms up to and including n */
static void compute_upto(idx_t n)
{
    /* initialise for first two terms if needed */
    if (seq_len == 0) {
        seq_ensure(2);
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        seq_len = 2;

        sa_init();
        sa_extend(0);  /* bit for a(0) */
        sa_extend(1);  /* bit for a(1) */
    }

    if ((idx_t)seq_len > n) return;

    /* allocate new terms */
    seq_ensure((size_t)n + 1);
    for (idx_t i = seq_len; i <= n; ++i) {
        char pat[65];
        bin_str(i, pat);
        size_t plen = strlen(pat);

        bool exists = pat_exists(pat, plen);
        seq[i] = exists ? (seq_t)1 : (seq_t)0;
        sa_extend((int)(seq[i] == (seq_t)1 ? 1 : 0));
    }
    seq_len = (size_t)n + 1;
}

/* public sequence function */
seq_t a(idx_t n)
{
    compute_upto(n);
    return seq[(size_t)n];
}
