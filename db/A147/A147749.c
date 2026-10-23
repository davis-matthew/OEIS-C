/*
 * A147749  Eight-digit numbers with all distinct digits with property that
 *          the string formed by the last k digits is divisible by k for any
 *          1<=k<=8.
 * https://oeis.org/A147749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) computed by enumerating all 8‑digit numbers with distinct
 *            digits for which the suffix of length k is divisible by k for
 *            every 1 ≤ k ≤ 8. The sequence contains exactly 921 elements
 * Range:     u8 none, u16 none, u32 n <= 921, u64 n <= 921, u128 n <= 921,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t pow10[9];
static seq_t *seq_arr = NULL;
static size_t seq_count = 0;
static bool seq_initialized = false;

/* Test whether the suffix divisibility condition holds. */
static bool suffix_divisible(seq_t n)
{
    for (int k = 1; k <= 8; ++k)
    {
        seq_t suffix = n % pow10[k];
        if ((suffix % (seq_t)k) != 0)
            return false;
    }
    return true;
}

/* Recursive generation of all 8‑digit permutations with distinct digits
   and no leading zero.  When a full 8‑digit number is built, it is
   checked for the suffix property before being stored. */
static void generate_rec(int pos, seq_t num, bool used[10])
{
    if (pos == 8)
    {
        if (suffix_divisible(num))
            seq_arr[seq_count++] = num;
        return;
    }
    for (int d = 0; d <= 9; ++d)
    {
        if (!used[d])
        {
            if (pos == 0 && d == 0)   /* no leading zero */
                continue;
            used[d] = true;
            generate_rec(pos + 1, num * 10 + d, used);
            used[d] = false;
        }
    }
}

/* Generate the full sequence once.  It is called lazily from a(). */
static void generate_all(void)
{
    seq_arr = (seq_t *)malloc(sizeof(seq_t) * 921);
    pow10[0] = 1;
    for (int i = 1; i <= 8; ++i)
        pow10[i] = pow10[i - 1] * 10;
    bool used[10] = {0};
    generate_rec(0, 0, used);
    seq_initialized = true;
}

seq_t a(idx_t n)
{
    if (!seq_initialized)
        generate_all();
    if (n == 0 || (size_t)n > seq_count)
        return 0;          /* undefined for this index */
    return seq_arr[(size_t)n - 1];
}
