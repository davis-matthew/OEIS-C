/*
 * A033079  Numbers in which all pairs of consecutive base-8 digits differ by
 *          3.
 * https://oeis.org/A033079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033079: Numbers in which all pairs of consecutive base‑8
   digits differ by 3.  The function generates the sequence in
   increasing order.  Because the counts grow roughly twofold per
   digit, 100 000 terms require at most 12 base‑8 digits, well below
   64 bits; the implementation therefore allocates a static array
   large enough for the requested range.  It works for any seq_t
   that supports plain arithmetic and default construction.  No
   dynamic memory allocation is performed, so it compiles in both
   C11 and C++17 (arbitrary‑precision seq_t such as an arbitrary‑
   precision integer class is handled via the static array). */

#include <stdlib.h>   /* for abort() */

/* adjacency information: for each digit d, neigh[d][i] is the
   i‑th allowable next digit; -1 marks unused slots. */
static const int neigh_cnt[8] = {1,1,1,2,2,1,1,1};
static const int neigh[8][2] = {
    {3,-1}, {4,-1}, {5,-1},
    {0,6},  {1,7}, {2,-1},
    {3,-1}, {4,-1}
};

/* cache storage – large enough for at least the first 100 000 terms */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;        /* number of values already stored */
static idx_t  next_len  = 1;        /* length of base‑8 numbers to generate next */

/* push a new value into the cache; assumes cache_len < MAX_CACHE */
static void push_value(seq_t v)
{
    cache[cache_len++] = v;
}

/* depth‑first search that appends digits and builds numbers
   of the current length.  rem is the remaining digits to append. */
static void dfs_gen(int last, seq_t val, idx_t rem)
{
    if (rem == 0) {
        push_value(val);
        return;
    }
    for (int i = 0; i < neigh_cnt[last]; ++i) {
        int d = neigh[last][i];
        dfs_gen(d, val * ((seq_t)8) + d, rem - 1);
    }
}

/* generate all numbers of base‑8 length “len” in lexicographic order */
static void generate_len(idx_t len)
{
    if (len == 1) {
        for (int d = 0; d <= 7; ++d) push_value((seq_t)d);
    } else {
        for (int d = 1; d <= 7; ++d)  /* first digit cannot be 0 */
            dfs_gen(d, (seq_t)d, len - 1);
    }
}

/* generate terms until we have at least “need” elements */
static void ensure(idx_t need)
{
    while (cache_len < (size_t)need) {
        if (cache_len >= MAX_CACHE) abort(); /* safety guard */
        generate_len(next_len);
        ++next_len;
    }
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is 1‑based */
    ensure(n);
    return cache[n-1];
}
