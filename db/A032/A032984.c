/*
 * A032984  Numbers with the property that all pairs of consecutive base-7
 *          digits differ by more than 2.
 * https://oeis.org/A032984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032984: Numbers whose consecutive base‑7 digits differ by more than 2.
   This implementation builds the sequence lazily by generating all
   admissible base‑7 numbers in increasing length.  It stores the
   generated values in a static cache so that subsequent calls to a(n)
   access the stored entry directly.  The cache holds at most 200,000
   terms, which is far more than enough for n ≤ 10,000. */

#include <stddef.h>   /* size_t */

#define G_CACHE_CAP 200000

/* static cache */
static seq_t g_cache[G_CACHE_CAP];
static size_t g_cache_len = 0;

/* last generated base‑7 length (1 for single‑digit numbers) */
static idx_t g_last_len = 0;

/* Adjacency lists for digits 0 … 6.
   Only the first neighborCount[d] entries are valid.                */
static const uint8_t neighborCount[7] = {4,3,2,2,2,3,4};
static const uint8_t neighborList[7][4] = {
    {3,4,5,6},     /* 0 : |0‑x| > 2  → 3,4,5,6   */
    {4,5,6,0},     /* 1 : |1‑x| > 2  → 4,5,6    (0 unused) */
    {5,6,0,0},     /* 2 : |2‑x| > 2  → 5,6      (0 unused) */
    {0,6,0,0},     /* 3 : 0,6                      */
    {0,1,0,0},     /* 4 : 0,1                      */
    {0,1,2,0},     /* 5 : 0,1,2                    */
    {0,1,2,3}      /* 6 : 0,1,2,3                  */
};

/* Recursive helper to generate all admissible numbers of a given length. */
static void generate_recursive(seq_t val, uint8_t prev, idx_t pos, idx_t len)
{
    if (pos == len) {
        /* store the generated number */
        g_cache[g_cache_len++] = val;
        return;
    }

    const uint8_t *nbr = neighborList[prev];
    size_t cnt = neighborCount[prev];
    for (size_t i = 0; i < cnt; ++i) {
        uint8_t nxt = nbr[i];
        generate_recursive(val * ((seq_t)7) + nxt, nxt, pos + 1, len);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* out of bounds */

    /* Generate terms until we have at least n of them */
    while (g_cache_len < n) {
        idx_t len = g_last_len + 1;                  /* next length */
        for (uint8_t d = 1; d <= 6; ++d) {           /* leading digit */
            generate_recursive((seq_t)d, d, 1, len);
        }
        g_last_len = len;
    }
    return g_cache[n - 1];
}
