/*
 * A152852  Numbers with all different digits such that each digit leaves the
 *          same nonzero remainder when each is divided into the number.
 * https://oeis.org/A152852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, full, fini
 * Formula:   numbers with all distinct non‑zero digits such that each digit
 *            leaves the same nonzero remainder when it divides the number.
 *            Computed by brute‑force enumeration of all permutations of
 *            digits 1…9 of length 1–9. The sequence has exactly forty terms
 * Range:     u8 none, u16 none, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Global state --------------------------------------------- */
static seq_t *all_numbers   = NULL;   /* array of all valid numbers */
static size_t all_cap       = 0;      /* current capacity */
static size_t all_len       = 0;      /* number of stored numbers */

static bool initialized      = false;
static seq_t cache[41];              /* cache[1] .. cache[40] */
static size_t term_count      = 0;    /* number of terms stored (≤ 40) */

/* Append a number to the growing array --------------------------------- */
static inline void add_number(seq_t num)
{
    if (all_len >= all_cap) {
        size_t new_cap = (all_cap == 0) ? 256 : all_cap * 2;
        seq_t *tmp = (seq_t*)realloc(all_numbers, new_cap * sizeof(seq_t));
        if (!tmp) return;          /* silently ignore out‑of‑memory */
        all_numbers = tmp;
        all_cap = new_cap;
    }
    all_numbers[all_len++] = num;
}

/* Recursive generation of permutations ------------------------------ */
static void recurse(int pos, int len, seq_t val, uint16_t mask, uint8_t *digits)
{
    if (pos == len) {
        /* evaluate the formed number */
        seq_t r = val % digits[0];
        if (r == 0) return;         /* remainder must be non‑zero */

        for (int i = 1; i < len; ++i)
            if (val % digits[i] != r)
                return;

        add_number(val);
        return;
    }

    for (uint8_t d = 1; d <= 9; ++d) {
        if (!(mask & (1u << d))) {
            digits[pos] = d;
            recurse(pos + 1, len, val * 10 + d, mask | (1u << d), digits);
        }
    }
}

/* Comparator for qsort --------------------------------------------- */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t x = *(const seq_t*)a;
    seq_t y = *(const seq_t*)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* One‑time initialization ------------------------------------------- */
static void init_cache(void)
{
    if (initialized) return;
    initialized = true;

    uint8_t digits[9];

    /* Generate all qualifying numbers */
    for (int len = 1; len <= 9; ++len)
        recurse(0, len, (seq_t)0, 0, digits);

    /* Sorting to obtain increasing order */
    if (all_len > 0)
        qsort(all_numbers, all_len, sizeof(seq_t), cmp_seq_t);

    /* Store the first 40 terms in the cache */
    term_count = (all_len < 40) ? all_len : 40;
    for (size_t i = 0; i < term_count; ++i)
        cache[i + 1] = all_numbers[i];

    /* Reclaim temporary storage */
    free(all_numbers);
    all_numbers = NULL;
    all_cap = 0;
    all_len = 0;
}

/* Public sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    init_cache();
    if (n == 0 || n > term_count) return (seq_t)0;
    return cache[n];
}
