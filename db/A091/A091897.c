/*
 * A091897  Numbers that remain prime or composite for all permutations of
 *          their digits.
 * https://oeis.org/A091897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   cmp_int, get_digits, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers that remain prime or composite for all permutations of
   their decimal digits. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_digits.h"
#include "../utils/seq_realloc.h"
#include "../utils/cmp_int.h"

static seq_t *terms   = NULL;   /* cached sequence terms */
static size_t  sz     = 0;      /* number of terms stored   */
static size_t  cap    = 0;      /* current capacity         */
static idx_t   cand   = 2;      /* next integer to test     */
static idx_t   max_seq_val = 0; /* maximum value representable by seq_t */

/* Generate the next lexicographic permutation of array a[0..n-1].
   Returns false if a is already the last permutation. */
static bool next_permutation(int *a, size_t n)
{
    if (n < 2) return false;
    int i = (int)n - 2;
    while (i >= 0 && a[i] >= a[i + 1]) i--;
    if (i < 0) return false;
    int j = (int)n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int left = i + 1, right = (int)n - 1; left < right; ++left, --right) {
        tmp = a[left]; a[left] = a[right]; a[right] = tmp;
    }
    return true;
}

/* Returns true iff every permutation of the decimal digits of n has
   the same primality status as n itself.
   Numbers whose permutations evaluate to 0 or 1 are automatically
   rejected because 0 and 1 are neither prime nor composite. */
static bool same_primality(seq_t n)
{
    bool n_is_prime = is_prime_big(n);

    /* Extract digits (least significant first). */
    uint8_t buf[128];
    size_t len = 0;
    get_digits(n, buf, &len);
    if (len == 0) return false; /* should not happen */

    /* Convert to an array of digits in most‑significant order. */
    int digits[128];
    for (size_t i = 0; i < len; ++i)
        digits[i] = buf[len - 1 - i];

    /* Start from the lowest lexicographic permutation. */
    qsort(digits, len, sizeof(int), cmp_int);

    do {
        seq_t val = 0;
        for (size_t i = 0; i < len; ++i)
            val = val * 10 + (seq_t)digits[i];

        if (val == 0 || val == (seq_t)1) return false;

        bool val_is_prime = is_prime_big(val);
        if (val_is_prime != n_is_prime)
            return false;
    } while (next_permutation(digits, len));

    return true;
}

/* Grow the sequence cache to hold at least n terms. */
static void build_up_to(idx_t n)
{
    if (n == 0) return;

    /* Determine the maximum value that fits in seq_t. */
    if (max_seq_val == 0) {
        seq_t max_seq = (seq_t)(-1);
        max_seq_val = (idx_t)max_seq;
    }

    /* Extend the cache. */
    while (sz < n) {
        if (cand > max_seq_val) break; /* cannot represent larger numbers */

        seq_t cand_seq = (seq_t)cand;
        if (same_primality(cand_seq)) {
            if (sz == cap) {
                size_t new_cap = (cap == 0) ? 32 : cap * 2;
                seq_t *tmp = seq_realloc(terms, cap, new_cap);
                if (!tmp) abort();
                terms = tmp;
                cap = new_cap;
            }
            terms[sz++] = cand_seq;
        }
        cand++;
    }
}

/* Public function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; just in case */
    build_up_to(n);
    return terms[n - 1];
}
