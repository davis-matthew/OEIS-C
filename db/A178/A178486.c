/*
 * A178486  (A178476(n)-3)/9.
 * https://oeis.org/A178486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base, easy
 * Formula:   a(n) = (p(n) - 3)/9, where p(n) is the nth smallest 6‑digit
 *            permutation of the digits 1,2,3,4,5,6
 * Range:     u8 none, u16 n <= 600, u32 n <= 720, u64 n <= 720, u128 n <= 720,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* --- pre‑computed values ----------------------------------------- */
static bool          _a178486_initialized = false;
static seq_t         _a178486_values[720];
static idx_t         _a178486_count;

/* --- recursion generating permutations in lexicographic order */
static void _a178486_gen_rec(int pos, idx_t cur, bool used[7])
{
    if (pos == 6) {
        _a178486_values[_a178486_count++] = (seq_t)((cur - (idx_t)3) / (idx_t)9);
        return;
    }
    for (int d = 1; d <= 6; ++d) {
        if (!used[d]) {
            used[d] = true;
            _a178486_gen_rec(pos + 1, cur * (idx_t)10 + (idx_t)d, used);
            used[d] = false;
        }
    }
}

/* --- initialise the memoised array ------------------------------ */
static void _a178486_init(void)
{
    bool used[7] = { false };
    _a178486_count = 0;
    _a178486_gen_rec(0, (idx_t)0, used);
    _a178486_initialized = true;
}

/* --- public sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (!_a178486_initialized)
        _a178486_init();

    if (n == 0)
        return (seq_t)0;

    if (n <= (idx_t)720)
        return _a178486_values[n - 1];

    /* out of bounds: undefined behaviour → return 0 */
    return (seq_t)0;
}
