/*
 * A093670  Numbers having a unique representation as ab+ac+bc, with 1 <= a <=
 *          b <= c.
 * https://oeis.org/A093670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A093670 : Numbers having a unique representation as ab+ac+bc
 *  with 1 <= a <= b <= c.
 *
 *  The sequence is computed by generating all values of
 *  ab+ac+bc up to a limit L, counting how many times each value
 *  occurs.  A value with count==1 is unique.  The list of unique
 *  values is sorted as the final sequence.
 *
 *  On each call to the function we enlarge L until we have at
 *  least the requested n‑th term.  The construction is
 *  deterministic and works for any call order.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void build_sequence_up_to(idx_t limit, seq_t **out_vals, size_t *len)
{
    /* count array: 0..limit */
    uint16_t *cnt = (uint16_t *)calloc(limit + 1, sizeof(uint16_t));
    if (!cnt) return;

    /* enumeration of triples (a,b,c), 1 <= a <= b <= c */
    for (uint64_t a = 1; 3ULL * a * a <= limit; ++a)
    {
        for (uint64_t b = a; ; ++b)
        {
            uint64_t f_min = 2ULL * a * b + b * b;      /* with c = b */
            if (f_min > limit) break;                   /* no larger b will work */

            for (uint64_t c = b; ; ++c)
            {
                uint64_t val = a * b + a * c + b * c;
                if (val > limit) break;
                if (cnt[val] < 65535) ++cnt[val];        /* we only need to know >1 */
            }
        }
    }

    /* collect values occurring exactly once */
    size_t estimated = limit / 4 + 10;          /* rough upper bound */
    seq_t *list = mem_new(seq_t, estimated);
    size_t size = 0;

    for (uint64_t v = 3; v <= limit; ++v)       /* minimum is 3 (1*1+1*1+1*1) */
    {
        if (cnt[v] == 1)
        {
            if (size == estimated)
            {
                estimated = (estimated * 3) / 2 + 10;
                list = mem_resize(seq_t, list, estimated);
            }
            list[size++] = (seq_t)v;
        }
    }

    free(cnt);
    *out_vals = list;
    *len = size;
}

/* ---------- private helpers (local to this file) ---------- */

/* ---------- public function required by the build ---------- */

seq_t a(idx_t n)
{
    static idx_t current_limit = 0;    /* last limit used for construction */
    static seq_t *values = NULL;
    static size_t values_len = 0;

    if (n == 0) return (seq_t)0;          /* undefined, but safe */

    while (values_len < (size_t)n)
    {
        idx_t new_limit = current_limit == 0 ? 2000000ULL : current_limit * 2ULL;
        seq_t *new_vals = NULL;
        size_t new_len = 0;

        build_sequence_up_to(new_limit, &new_vals, &new_len);

        if (values) mem_free(values);
        values = new_vals;
        values_len = new_len;
        current_limit = new_limit;
    }

    return values[n - 1];
}
