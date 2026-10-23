/*
 * A045060  Numbers whose base-4 representation contains exactly four 0's and
 *          three 2's.
 * https://oeis.org/A045060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045060: Numbers whose base‑4 representation contains exactly
   four 0's and three 2's.  The code enumerates all base‑4 numbers of
   increasing length, ensuring the most significant digit is non‑zero,
   and counts exactly 4 zeros and 3 twos.  The numbers are generated in
   ascending order, so a(n) can be accessed directly. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed terms (1‑based) */
static size_t cap    = 0;     /* current capacity of cache        */
static size_t stored = 0;     /* number of terms stored           */
static idx_t next_len = 7;    /* smallest length still to generate */

/* Append a new value to the cache, growing it if necessary. */
static void add_value(seq_t val)
{
    if (stored == cap) {
        size_t new_cap = (cap == 0 ? 64 : cap * 2);
        seq_t *p = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!p) abort();  /* should not happen in valid environments */
        cache = p;
        cap = new_cap;
    }
    cache[++stored] = val;
}

/* Recursive helper that builds base‑4 numbers of length L,
   having zeros_left zeros and twos_left twos remaining, and
   the currently accumulated value `val`. */
static void gen(idx_t pos,
                idx_t L,
                seq_t val,
                int zeros_left,
                int twos_left)
{
    if (pos == L) {
        if (zeros_left == 0 && twos_left == 0)
            add_value(val);
        return;
    }

    int start_digit = (pos == 0) ? 1 : 0;  /* most significant digit cannot be 0 */
    for (int d = start_digit; d <= 3; ++d) {
        if (d == 0 && zeros_left == 0) continue;
        if (d == 2 && twos_left == 0)  continue;

        int z_left = zeros_left - (d == 0);
        int t_left = twos_left  - (d == 2);
        idx_t remaining = L - pos - 1;

        if ((idx_t)z_left > remaining) continue;
        if ((idx_t)t_left > remaining) continue;
        if ((idx_t)(z_left + t_left) > remaining) continue;

        seq_t next_val = val * 4 + d;
        gen(pos + 1, L, next_val, z_left, t_left);
    }
}

/* Generate all valid numbers of a particular length L and append them
   to the cache. */
static void generate_len(idx_t L)
{
    gen(0, L, 0, 4, 3);
}

/* Public entry point required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* the sequence is 1‑based; unused case */

    /* Generate additional terms until we have at least n values. */
    while ((idx_t)stored < n) {
        generate_len(next_len);
        ++next_len;
    }
    return cache[n];
}
