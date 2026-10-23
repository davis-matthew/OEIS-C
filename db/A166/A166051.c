/*
 * A166051  Nonsquare integers of the form 4n+1 for which Sum_{i=1..u}
 *          J(i,4n+1) is never negative for any u in range [1,(2n)], where
 *          J(i,k) is the Jacobi symbol.
 * https://oeis.org/A166051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth nonsquare integer of the form 4k+1 whose *
 *            cumulative Jacobi sums Σ_{i=1..u} J(i,4k+1) are never negative
 *            for * any u in [1,2k]. The function discovers these terms by
 *            brute force * search; it does not use a pre‑computed lookup
 *            table
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/jacobi_symbol.h"

static inline bool is_square_uint64(uint64_t x)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        __int128 prod = (__int128)mid * (__int128)mid;
        if (prod >= (__int128)x)
            hi = mid;
        else
            lo = mid + 1;
    }
    return ((__uint128_t)lo * (__uint128_t)lo == (__uint128_t)x);
}

/* Small cache for discovered terms (there are only seven known). */
static seq_t terms[128];
static size_t terms_len = 0;
/* The next candidate k to examine.  We start with k = 1 (m = 5). */
static idx_t last_k = 1;

/* Generate the next term that satisfies the definition.  The caller
 * should ensure that this function is not called endlessly if no
 * further terms exist.  A hard‑coded safety limit prevents infinite
 * loops when the sequence is finite. */
static void generate_next(void)
{
    for (;;) {
        seq_t m = (seq_t)4 * (seq_t)last_k + 1;
        uint64_t m64 = (uint64_t)m;

        /* Skip perfect squares. */
        if (!is_square_uint64(m64)) {
            int sum = 0;
            bool ok = true;
            for (idx_t i = 1; i <= 2 * last_k; ++i) {
                int s = jacobi_symbol(i, m64);
                sum += s;
                if (sum < 0) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                if (terms_len < sizeof(terms) / sizeof(*terms))
                    terms[terms_len++] = m;
                ++last_k;              /* advance for next candidate */
                return;
            }
        }
        ++last_k;                  /* reject current candidate */
        if (last_k > (idx_t)2000000)   /* safety cap for finite sequences */
            return;
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    while (terms_len < (size_t)n)
        generate_next();

    if (n <= terms_len)
        return terms[n - 1];

    /* Not enough terms found within the safety limit. */
    return (seq_t)0;
}
