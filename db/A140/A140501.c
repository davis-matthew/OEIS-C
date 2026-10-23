/*
 * A140501  Ten-digit numbers with all distinct digits such that the string
 *          formed by the last k digits is divisible by k for any k <= 10.
 * https://oeis.org/A140501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 202, u128 n <= 202,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates A140501: 10‑digit numbers with distinct digits such that
   the string formed by the last k digits is divisible by k for every k
   ≤ 10.  The sequence is produced by iterating all permutations of
   1–9 followed by 0 and keeping the numbers that satisfy the divisibility
   property.  a(1) is the smallest such number, and a(n) is the n‑th term
   in increasing numeric order. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static state for memoisation */
static seq_t *seq_cache = NULL;      /* array of found terms  */
static size_t seq_len   = 0;         /* number of terms stored   */
static size_t seq_cap   = 0;         /* capacity of the array    */

/* next_permutation for an array of int in C */
static bool next_permutation_int(int *a, int n)
{
    int i = n - 2;
    while (i >= 0 && a[i] >= a[i+1]) i--;
    if (i < 0) return false;
    int j = n - 1;
    while (a[j] <= a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = n - 1; l < r; l++, r--) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

/* Generate and memoise all terms of the sequence */
static void build_cache(void)
{
    int perm[9] = {1,2,3,4,5,6,7,8,9};
    do {
        bool ok = true;
        seq_t suffix = 0;
        seq_t pow10 = 1;
        /* test the suffix divisibility condition, building the whole number
           at the same time */
        for (int pos = 9; pos >= 0; --pos) {
            int digit   = (pos == 9 ? 0 : perm[pos]);
            suffix += (seq_t)digit * pow10;   /* suffix of length 10−pos  */
            int k = 10 - pos; /* length of the suffix considered */
            if (suffix % k != 0) { ok = false; break; }
            pow10 *= 10;
        }
        if (ok) {
            /* ensure space in the cache */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);            /* allocation failure */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = suffix;     /* store the found term */
        }
    } while (next_permutation_int(perm, 9));
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* offset is 1; 0 is undefined */
    static bool built = false;
    if (!built) { build_cache(); built = true; }
    if (n - 1 < seq_len) return seq_cache[n - 1];
    return 0;                                      /* out of range: value undefined */
}
