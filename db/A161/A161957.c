/*
 * A161957  Fixed points of A161955.
 * https://oeis.org/A161957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   bitlen_seq_t, factor_u64, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161957: Fixed points of the binary TITO2 operation.
   There is no closed formula; we generate the terms by brute‑force
   testing odd integers until the requested index is reached.  The
   property is
        a = product over prime factors p^e of reverse_bin(p)^e
        and reverse_bin(a) == that product,
   where reverse_bin(x) is the bitwise reversal of x’s binary
   representation (without leading zeros).  That is what the routine
   below implements.                           */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/bitlen_seq_t.h"
#include "../utils/grow_seq_array.h"

static inline seq_t reverse_bits_seq(seq_t x)
{
    unsigned len = bitlen_seq_t(x);
    seq_t rev = 0;
    for (unsigned i = 0; i < len; ++i) {
        rev <<= 1;
        rev |= (x & 1);
        x >>= 1;
    }
    return rev;
}

static bool is_fixed_point(seq_t value)
{
    /* value is expected to fit into a 64‑bit integer. */
    uint64_t num = (uint64_t)value;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(num, pr, ex, &cnt);

    seq_t rev_x = reverse_bits_seq(num);
    seq_t rev_prod = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t rev_p = reverse_bits_seq(pr[i]);
        for (uint64_t j = 0; j < ex[i]; ++j) {
            /* Prevent overflow or too large intermediate product. */
            if (rev_p != 0 && rev_prod > rev_x / rev_p)
                return false;
            rev_prod *= rev_p;
        }
    }
    return rev_prod == rev_x;
}

static seq_t *seq_cache = NULL;
static idx_t seq_len = 0;
static idx_t seq_cap = 0;
static idx_t next_candidate = 1;

seq_t a(idx_t n)
{
    /* Ensure enough terms are precomputed. */
    while (seq_len < n) {
        /* Find the next odd candidate that is a fixed point. */
        while (!is_fixed_point(next_candidate))
            next_candidate += 2;              /* skip evens */

        /* Grow the cache if necessary. */
        if (seq_len >= seq_cap)
            grow_seq_array(&seq_cache, &seq_cap, seq_len + 1);

        seq_cache[seq_len++] = (seq_t)next_candidate;
        next_candidate += 2;                  /* next odd */
    }
    return seq_cache[n - 1];
}
