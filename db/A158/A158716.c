/*
 * A158716  Largest prime with n distinct nonprime digits.
 * https://oeis.org/A158716
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by generating all permutations of the non‑prime digits
   {0,1,4,6,8,9} and selecting the largest prime containing exactly
   n distinct non‑prime digits.  For n>6 the set is empty, so the
   result is zero. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

static const int NON_PRIME_DIGITS[6] = {0, 1, 4, 6, 8, 9};

static uint64_t best_val[7] = {0};      /* best prime for each length 0–6   */
static seq_t   memo_val[7] = {0};      /* result cache                     */
static bool    computed = false;       /* one‑time initialization          */
static bool    used[6] = {0};          /* helper for permutation generation */

static void generate(int depth, uint64_t val)
{
    if (depth >= 2) {
        if (is_prime(val)) {
            if (val > best_val[depth])
                best_val[depth] = val;
        }
    }
    if (depth == 6)            /* maximum length reached */
        return;

    for (int i = 0; i < 6; ++i) {
        if (!used[i]) {
            int d = NON_PRIME_DIGITS[i];
            if (depth == 0 && d == 0)   /* no leading zero */
                continue;
            used[i] = true;
            generate(depth + 1, val * 10 + (uint64_t)d);
            used[i] = false;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2 || n > 6)
        return (seq_t)0;

    if (!computed) {
        /* run the search only once */
        for (int i = 0; i < 6; ++i)
            used[i] = false;
        for (int i = 0; i < 7; ++i)
            best_val[i] = 0;
        generate(0, 0);
        for (int i = 2; i <= 6; ++i)
            memo_val[i] = (seq_t)best_val[i];
        computed = true;
    }
    return memo_val[n];
}
