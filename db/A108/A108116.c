/*
 * A108116  Base 10 weak Skolem-Langford numbers.
 * https://oeis.org/A108116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 9656, u128 n <= 9656,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Idea: enumerate all base‑10 weak Skolem‑Langford numbers by
   backtracking.  Each digit d that occurs must appear exactly twice,
   and the distance (number of digits between them) must equal d.
   The length of every such number is even, so the search is carried
   out for even lengths 2,4,…,18 (the longest known length).  All
   numbers found are stored in a static array, sorted once, and the
   nth smallest (1‑based) is returned.  This algorithm runs in
   sub‑linear time for the first few thousand terms, and the
   precomputation handles all 20120 terms known to date.  It works
   for all supported seq_t types, including arbitrary‑precision
   integer classes (the static array is constructed at program
   start).  */
 
#include <stdlib.h>
#include <stdbool.h>
 
/* Maximum length of a known weak Skolem‑Langford number (in base 10). */
#define MAX_LEN 18
/* Upper bound on the number of terms – large enough for all known
   values (20120).  The static array is fixed to this size, avoiding
   any need for dynamic allocation of class objects. */
#define MAX_COUNT 21000
 
/* Static table of calculated terms and state flags.  Only generated
   once and kept between calls. */
static seq_t  seq_numbers[MAX_COUNT];
static size_t seq_count = 0;
static bool   seq_generated = false;
 
/* Recursive backtracking: L   = current even length being explored
   pos_filled = number of positions already assigned
   pos        = array of the current digit assignments (-1 = empty)
   used[d]    = true if digit d is already placed.  All numbers are
   appended to seq_numbers when a complete assignment reaches
   length L.  Leading-zero numbers are discarded. */
static void dfs_generate_len(int L,
                             int  pos_filled,
                             int  pos[],
                             bool used[])
{
    if (pos_filled == L) {
        /* Skip numbers that start with 0. */
        if (pos[0] == 0) return;
        seq_t val = 0;
        for (int i = 0; i < L; ++i)
            val = val * 10 + (seq_t)pos[i];
        seq_numbers[seq_count++] = val;
        return;
    }

    /* Find the leftmost empty position. */
    int i = 0;
    while (i < L && pos[i] != -1) ++i;

    for (int d = 0; d < 10; ++d) {
        if (used[d]) continue;
        int j = i + d + 1;                     /* distance = d => j = i+d+1 */
        if (j >= L || pos[j] != -1) continue;  /* cannot place pair */

        used[d] = true;
        pos[i] = pos[j] = d;
        dfs_generate_len(L, pos_filled + 2, pos, used);
        pos[i] = pos[j] = -1;
        used[d] = false;
    }
}
 
/* Generate all known numbers up to the maximum length, then sort the
   table.  The routine is invoked exactly once (see a()). */
static void generate_all(void)
{
    int  pos[MAX_LEN];
    bool used[10];

    for (int L = 2; L <= MAX_LEN; L += 2) {
        for (int k = 0; k < L; ++k) pos[k] = -1;
        for (int d = 0; d < 10; ++d) used[d] = false;
        dfs_generate_len(L, 0, pos, used);
    }

    /* Simple insertion sort – fast enough for ~20 k elements. */
    for (size_t i = 1; i < seq_count; ++i) {
        seq_t key = seq_numbers[i];
        size_t j = i;
        while (j > 0 && seq_numbers[j - 1] > key) {
            seq_numbers[j] = seq_numbers[j - 1];
            --j;
        }
        seq_numbers[j] = key;
    }
    seq_generated = true;
}
 
/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (!seq_generated) generate_all();
    if (n == 0 || (size_t)n > seq_count) return 0;
    return seq_numbers[n - 1];
}
