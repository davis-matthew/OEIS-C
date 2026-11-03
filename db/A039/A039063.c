/*
 * A039063  Numbers whose base-8 representation has the same number of 2's and
 *          4's.
 * https://oeis.org/A039063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   This function enumerates numbers whose base‑8 representation has
 *            the same number of digits 2 and 4. It stores on first use the
 *            sequence values in a table so that any n ≥ 1 can be returned in
 *            O(1) after an initial pass. The table is grown as needed
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether an integer x has an equal number of 2's and 4's in octal */
static bool has_equal_two_four(idx_t x)
{
    int c2 = 0, c4 = 0;
    while (x >> 0) {           /* loop until x becomes 0 */
        int d = (int)(x & 7);  /* least octal digit (x % 8)   */
        if (d == 2) ++c2;
        else if (d == 4) ++c4;
        x >>= 3;               /* divide by 8                  */
    }
    return c2 == c4;
}

/* internal helpers ----------------------------------------------------- */

/* static table of computed sequence values ----------------------------- */
static seq_t *seq_cache   = NULL;   /* array of values          */
static size_t  cache_cap  = 0;      /* allocated capacity       */
static size_t  cache_size = 0;      /* how many values stored   */
static idx_t   next_val   = 0;      /* next integer to test     */

/* grow the table if needed ------------------------------------------- */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    if (!tmp) { mem_free(seq_cache); exit(1); }
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* the sequence is defined for n ≥ 1; behaviour for n==0 is non‑standard */
    if (n == 0) return 0;

    /* generate more terms if required */
    while (cache_size < (size_t)n) {
        ensure_cache_capacity(cache_size + 1);
        if (has_equal_two_four(next_val)) {
            seq_cache[cache_size++] = (seq_t)next_val;
        }
        ++next_val;
    }
    return seq_cache[(size_t)n - 1];
}
