/*
 * A102660  List of Lyndon words on {1,2,3} sorted first by length and then
 *          lexicographically.
 * https://oeis.org/A102660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th Lyndon word over the alphabet {1,2,3} sorted
 *            first by length and then lexicographically. The word is returned
 *            as a decimal integer whose digits are 1, 2, 3
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 9382, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9103 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* ------------------------------------------------------------------ */
static bool is_lyndon(const int *d, idx_t L)
{
    for (idx_t shift = 1; shift < L; ++shift)
    {
        idx_t i;
        for (i = 0; i < L; ++i)
        {
            int a = d[i];
            int b = d[(i + shift) % L];
            if (a < b)           /* word < rotation → ok, continue to next shift */
                break;
            if (a > b)           /* rotation < word → not Lyndon */
                return false;
        }
        if (i == L)              /* rotation == word → periodic → not Lyndon */
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
static bool gen(idx_t depth, idx_t L, seq_t val, int *d,
                idx_t *global_count, idx_t target, seq_t *result)
{
    if (depth == L)
    {
        if (is_lyndon(d, L))
        {
            (*global_count)++;
            if (*global_count == target)
            {
                *result = val;
                return true;
            }
        }
        return false;
    }

    for (int digit = 1; digit <= 3; ++digit)
    {
        d[depth] = digit;
        seq_t new_val = val * 10 + digit;
        if (gen(depth + 1, L, new_val, d, global_count, target, result))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Helper: test whether the word stored in 'd[0..L-1]' is a Lyndon word */

/* ------------------------------------------------------------------ */
/* Recursive generator of all words of a fixed length L
   The words are explored in lexicographic order.
   When the global counter reaches the target rank, the current
   word (as a decimal integer) is stored in *result. */

/* ------------------------------------------------------------------ */
/* Main sequence function */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t global_count = 0;
    seq_t result = 0;
    int digits[20];               /* maximum length needed for n up to 10000 */

    for (idx_t L = 1; ; ++L)
    {
        if (gen(0, L, 0, digits, &global_count, n, &result))
            return result;
    }
}
