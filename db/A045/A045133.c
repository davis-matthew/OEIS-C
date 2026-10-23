/*
 * A045133  Numbers whose base-4 representation contains exactly four 1's and
 *          four 3's.
 * https://oeis.org/A045133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all base‑4 integers that have exactly
   four 1's and four 3's, in increasing numeric order.
   a(1) is the first such integer (22015). */

#include <stdlib.h>

/* Cache for memoised terms.  Initially empty. */
static seq_t *cache     = NULL;   /* raw storage for seq_t objects   */
static size_t cache_cap = 0;      /* allocated capacity in elements  */
static size_t cache_size = 0;     /* number of terms actually stored */

/* Current minimum length (in base‑4 digits) to generate.
   Numbers with exactly four 1's and four 3's must have
   at least 8 digits, so start at 8. */
static int current_len = 8;

/* Append a value to the cache, growing it when necessary. */
static void push_cache(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Recursive generator for numbers of a fixed length L.
   `pos`   : current position (0 = most significant digit)
   `ones`  : number of 1's seen so far
   `threes`: number of 3's seen so far
   `val`   : value accumulated so far (in base‑4)           */
static void gen_len_rec(int pos, int L, int ones, int threes, seq_t val)
{
    if (pos == L) {
        if (ones == 4 && threes == 4) push_cache(val);
        return;
    }

    int min_d = (pos == 0) ? 1 : 0;          /* leading digit cannot be 0 */
    for (int d = min_d; d <= 3; ++d) {
        int ones2   = ones   + (d == 1);
        int threes2 = threes + (d == 3);
        if (ones2 > 4 || threes2 > 4) continue;

        int need_ones   = 4 - ones2;
        int need_threes = 4 - threes2;
        int remaining   = L - pos - 1;
        if (need_ones + need_threes > remaining) continue;

        seq_t val2 = val * 4 + (seq_t)d;
        gen_len_rec(pos + 1, L, ones2, threes2, val2);
    }
}

/* Ensure that the cache contains at least n terms.
   Generates additional lengths as required, always finishing each
   length completely before proceeding to the next. */
static void generate_up_to(idx_t n)
{
    while (cache_size < n) {
        gen_len_rec(0, current_len, 0, 0, (seq_t)0);
        ++current_len;
    }
}

/* Public sequence function.  a(0) is undefined; return 0 for safety. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; 0 is unused */
    generate_up_to(n);
    return cache[n - 1];
}
