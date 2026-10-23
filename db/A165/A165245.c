/*
 * A165245  Nonprimes with all digits distinct, all digits prime, and a
 *          nonprime number of digits.
 * https://oeis.org/A165245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   a(n) is the nth smallest composite number with distinct prime
 *            digits {2,3,5,7} and a nonprime number of digits. Only 4‑digit
 *            numbers satisfy the digit length condition, so the sequence
 *            consists of all composite permutations of 2,3,5,7
 * Range:     u8 none, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Temporary storage for all 24 permutations and the final 16 terms. */
static seq_t all_perms[24];
static size_t all_count = 0;
static seq_t comps[16];
static size_t comp_count = 0;
static bool initialized = false;

/* Return true if n is prime (works for all values up to the 4‑digit
   range we handle here).  seq_t is unsigned, so no negative values
   arise. */
static bool is_prime_small(seq_t n)
{
    if (n < 2) return false;
    for (seq_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

/* Generate all 24 permutations of the digits {2,3,5,7} using a
   simple recursive routine.  The results are stored in all_perms. */
static void gen_permutations(int depth, int used_mask, int cur)
{
    if (depth == 4)
    {
        all_perms[all_count++] = (seq_t)cur;
        return;
    }

    /* Digits in sorted order: 2,3,5,7 */
    const int digits[4] = {2, 3, 5, 7};

    for (int i = 0; i < 4; ++i)
    {
        if (!(used_mask & (1 << i)))
        {
            gen_permutations(depth + 1, used_mask | (1 << i), cur * 10 + digits[i]);
        }
    }
}

/* Sort an array of seq_t values in ascending order for up to 24 items.
   Simple insertion sort is more than fast enough. */
static void sort_seq(seq_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i)
    {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key)
        {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* One‑time initialization: generate all permutations, keep composites,
   sort them, and store in comps[]. */
static void init_sequence(void)
{
    if (initialized) return;

    gen_permutations(0, 0, 0);
    comp_count = 0;
    for (size_t i = 0; i < all_count; ++i)
    {
        seq_t val = all_perms[i];
        if (!is_prime_small(val))
        {
            comps[comp_count++] = val;
        }
    }

    /* comp_count should be 16 for the given set of digits. */
    sort_seq(comps, comp_count);
    initialized = true;
}

/* Required function: return the nth term (1‑based).  If n exceeds the
   number of known terms, return 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* fallback for undefined n=0 */
    init_sequence();
    if ((idx_t)n <= comp_count)
        return comps[(size_t)(n - 1)];
    return 0;
}
