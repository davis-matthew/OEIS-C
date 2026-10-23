/*
 * A117911  Number of n-digit Lynch-Bell numbers.
 * https://oeis.org/A117911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   _gcd_u32, _lcm_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A117911 – Number of n‑digit Lynch‑Bell numbers
 *
 *  A Lynch‑Bell number is an integer whose every non‑zero digit divides
 *  the number itself.  All digits must be distinct and chosen from 1…9.
 *  Consequently the maximum length is 7; all longer numbers are impossible.
 *
 *  The routine enumerates all permutations of distinct digits 1‑9 up to
 *  length 7 (at most 9! = 362880 permutations) and counts those
 *  that satisfy the divisibility condition.  The counts are computed
 *  once on the first call and cached for subsequent calls.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>   /* size_t */
#include "../utils/_gcd_u32.h"
#include "../utils/_lcm_u32.h"

/*  Recursive enumeration of all permutations of distinct digits 1‑9
 *  (up to length 7).  `cnt` must be initialised with zeros before the first
 *  call. */
static void _dfs(uint32_t num, uint32_t lcm, unsigned mask, int depth, seq_t *cnt)
{
    /* depth : number of digits already chosen (0‑based) */
    if (depth == 7) return;               /* cannot exceed length 7 */

    for (int d = 1; d <= 9; ++d) {
        unsigned bit = 1u << (d - 1);
        if (mask & bit) continue;         /* digit already used */

        uint32_t new_num   = num * 10u + (uint32_t)d;
        uint32_t new_lcm   = _lcm_u32(lcm, (uint32_t)d);
        int      len       = depth + 1;   /* length of the new number */

        if ((new_num % new_lcm) == 0) {
            cnt[(size_t)len] = cnt[(size_t)len] + (seq_t)1;
        }
        _dfs(new_num, new_lcm, mask | bit, depth + 1, cnt);
    }
}

/* Initialise the count table */
static void _init_counts(seq_t *cnt)
{
    for (int i = 0; i < 8; ++i) cnt[(size_t)i] = 0;
    _dfs(0u, 1u, 0u, 0, cnt);
}

/* External function required by the output format */
seq_t a(idx_t n)
{
    static bool      init_done = false;
    static seq_t     cnt[8];

    if (!init_done) {
        _init_counts(cnt);
        init_done = true;
    }

    if (n < (idx_t)8) return cnt[(size_t)n];
    return (seq_t)0;
}
