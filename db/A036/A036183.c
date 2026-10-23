/*
 * A036183  Log base 2 (n) mod 107.
 * https://oeis.org/A036183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the discrete logarithm of n to base 2 modulo 107 i.e. the
 *            smallest non‑negative integer k such that 2^k ≡ n (mod 107) if
 *            no such k exists (n ≡ 0 mod 107) the function returns 0
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool _log2_107_inited = false;
static seq_t _log2_107_table[107];
static bool _log2_107_set[107];

seq_t a(idx_t n)
{
    /* One‑time construction of the discrete log table. */
    if (!_log2_107_inited) {
        unsigned i;

        /* Initialise flags. */
        for (i = 0; i < 107; ++i)
            _log2_107_set[i] = false;

        /* 2^0 ≡ 1 (mod 107). */
        _log2_107_table[1] = 0;
        _log2_107_set[1]  = true;

        /* Build the rest of the table, keeping the smallest exponent. */
        seq_t v = 1;
        for (i = 1; i < 107; ++i) {
            v = (v * (seq_t)2) % (seq_t)107;
            unsigned idx = (unsigned)v;             /* v is in 0..106 */
            if (!_log2_107_set[idx]) {
                _log2_107_table[idx] = i;
                _log2_107_set[idx]  = true;
            }
        }
        _log2_107_inited = true;
    }

    /* Reduce the argument modulo 107. */
    idx_t r = n % 107;
    if (r == 0)                     /* no discrete log for zero residue   */
        return 0;

    /* Retrieve the precomputed exponent. */
    return _log2_107_table[(unsigned)r];
}
