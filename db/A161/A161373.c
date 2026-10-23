/*
 * A161373  "Early bird" binary numbers: write the natural numbers in binary
 *          representation in a string 011011100101110111.... Sequence gives
 *          numbers which occur in the string strictly ahead of their natural
 *          place.
 * https://oeis.org/A161373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161373 – Early bird binary numbers
   Definition: a(n) is the n‑th natural number whose binary representation
   appears as a substring of the concatenation of all previous natural
   numbers written in binary.  The function generates the sequence
   incrementally using a suffix automaton that contains all substrings
   of the concatenated string processed so far.  The automaton is
   extended after each number.  The implementation is O(total number
   of bits) and works for any idx_t/seq_t pair. */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
 
/* ---------- suffix automaton for binary strings ---------- */
struct SAState {
    int link;
    int next[2];
    int len;
};
 
static struct SAState *sa = NULL;      /* array of states */
static size_t sa_cnt = 0;              /* number of states used */
static size_t sa_cap = 0;              /* capacity of sa array */
static int sa_last = 0;                /* last state */
 
static void sa_init(void)
{
    sa_cap = 1024;
    sa = (struct SAState *)malloc(sa_cap * sizeof(*sa));
    sa[0].len   = 0;
    sa[0].link  = -1;
    sa[0].next[0] = sa[0].next[1] = -1;
    sa_cnt = 1;
    sa_last = 0;
}
 
static void sa_extend(int bit)
{
    if (sa_cnt == sa_cap) {
        sa_cap <<= 1;
        sa = (struct SAState *)realloc(sa, sa_cap * sizeof(*sa));
    }
    int cur = (int)sa_cnt++;
    sa[cur].len = sa[sa_last].len + 1;
    sa[cur].next[0] = sa[cur].next[1] = -1;
 
    int p = sa_last;
    while (p != -1 && sa[p].next[bit] == -1) {
        sa[p].next[bit] = cur;
        p = sa[p].link;
    }
 
    if (p == -1) {
        sa[cur].link = 0;
    } else {
        int q = sa[p].next[bit];
        if (sa[p].len + 1 == sa[q].len) {
            sa[cur].link = q;
        } else {
            if (sa_cnt == sa_cap) {
                sa_cap <<= 1;
                sa = (struct SAState *)realloc(sa, sa_cap * sizeof(*sa));
            }
            int clone = (int)sa_cnt++;
            sa[clone].len   = sa[p].len + 1;
            sa[clone].next[0] = sa[q].next[0];
            sa[clone].next[1] = sa[q].next[1];
            sa[clone].link = sa[q].link;
            while (p != -1 && sa[p].next[bit] == q) {
                sa[p].next[bit] = clone;
                p = sa[p].link;
            }
            sa[q].link = sa[cur].link = clone;
        }
    }
    sa_last = cur;
}
 
static bool sa_contains(const int *bits, size_t len)
{
    int state = 0;
    for (size_t i = 0; i < len; ++i) {
        int b = bits[i];
        state = sa[state].next[b];
        if (state == -1) return false;
    }
    return true;
}
 
/* ---------- early‑bird number storage ---------- */
static seq_t *early = NULL;      /* array of early birds */
static size_t early_cnt = 0;     /* how many collected so far */
static size_t early_cap = 0;     /* capacity of array */
 
/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialisation on first call */
    static idx_t next_num = 0;          /* next natural number to process */
    if (early == NULL) {
        early_cap = 16;
        early = (seq_t *)malloc(early_cap * sizeof(seq_t));
    }
    if (sa == NULL) sa_init();
 
    /* generate early birds until we have at least n of them */
    while (early_cnt < (size_t)n) {
        /* obtain binary bits of next_num */
        int bits[65];
        size_t len = 0;
        if (next_num == 0) {
            bits[0] = 0;
            len = 1;
        } else {
            idx_t t = next_num;
            int msb = 63 - __builtin_clzll((unsigned long long)t);
            for (int i = msb; i >= 0; --i)
                bits[len++] = (t >> i) & 1U;
        }
 
        /* check if the bits appear earlier */
        if (sa_contains(bits, len)) {
            if (early_cnt == early_cap) {
                early_cap <<= 1;
                early = (seq_t *)realloc(early, early_cap * sizeof(seq_t));
            }
            early[early_cnt++] = (seq_t)next_num;
        }
 
        /* extend automaton with the bits of next_num */
        for (size_t i = 0; i < len; ++i)
            sa_extend(bits[i]);
 
        ++next_num;
    }
 
    return early[(size_t)n - 1];
}
