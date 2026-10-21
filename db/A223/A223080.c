/*
 * A223080  Numbers n with distinct digits such that the reversal of n divides
 *          n.
 * https://oeis.org/A223080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   rev, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A223080: numbers n with distinct digits such that rev(n) divides n.
   The function builds all such numbers once, stores them sorted, and returns
   the n‑th element. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility: reverse the decimal representation of an index. */
#include "../utils/rev.h"          /* rev(idx_t x) */

/* Utility: comparison function for qsort on seq_t values. */
#include "../utils/seq_cmp.h"      /* seq_cmp(const void*, const void*) */

/* Global cache of valid numbers */
static seq_t *val_list   = NULL;   /* array of found numbers */
static size_t val_count  = 0;      /* number of elements in the array */
static bool   initialized = false;

/* Recursive generation of all distinct‑digit numbers up to a given length. */
static void generate(int len, int pos, uint16_t used, seq_t cur)
{
    if (pos == len) {
        idx_t r = rev((idx_t)cur);          /* decimal reversal */
        if (r != 0 && cur % (seq_t)r == 0) { /* reverse divides the number */
            val_list[val_count++] = cur;
        }
        return;
    }

    for (int d = 0; d <= 9; ++d) {
        if (used & (1 << d))
            continue;                 /* digit already used */
        if (pos == 0 && d == 0)
            continue;                 /* no leading zero */
        generate(len, pos + 1, used | (1 << d), cur * 10 + d);
    }
}

/* One‑time initialization: generate all valid numbers up to 6 digits,
   sort them, and place them in the cache. */
static void init_sequence(void)
{
    /* Allocate enough space for all distinct‑digit numbers up to 6 digits
       (the total is 170 000; 200 k gives a comfortable margin). */
    val_list = (seq_t *)malloc(200000 * sizeof(seq_t));
    val_count = 0;

    for (int len = 1; len <= 6; ++len)
        generate(len, 0, 0, 0);

    /* Sort the list increasingly. */
    qsort(val_list, val_count, sizeof(seq_t), seq_cmp);
    initialized = true;
}

/* Public OEIS sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        init_sequence();

    if (n == 0 || n > val_count)
        return 0;                    /* out of bounds – not used. */

    return val_list[n - 1];          /* a(1) is the first term. */
}
