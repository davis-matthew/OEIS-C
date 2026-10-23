/*
 * A036743  Numbers <= 3999 sorted in Roman numeral reverse lexicographic
 *          order.
 * https://oeis.org/A036743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   cmp_reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036743: Numbers <=3999 sorted in Roman numeral reverse lexicographic order.
   The function precomputes all Roman numerals, sorts them (descending)
   lexicographically, and stores the sorted integer values in a static
   array for O(1) retrieval. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>   /* for uint16_t */
#include "../utils/cmp_reverse.h"

/* ------------------------------------------------------------- */
/* Helper: convert an integer (1..3999) to its Roman numeral.     */
static void int_to_roman(int n, char *buf)
{
    static const int values[]  = {1000,900,500,400,100,90,50,40,10,9,5,4,1};
    static const char *syms[] = {"M","CM","D","CD","C","XC","L","XL",
                                 "X","IX","V","IV","I"};
    char *p = buf;
    for (int i = 0; i < 13; ++i) {
        while (n >= values[i]) {
            const char *sym = syms[i];
            size_t len = strlen(sym);
            memcpy(p, sym, len);
            p += len;
            n -= values[i];
        }
    }
    *p = '\0';
}

/* ------------------------------------------------------------- */

/* ------------------------------------------------------------- */
/* Structures and static storage.                                 */
typedef struct {
    int    num;          /* integer value */
    char   roman[20];    /* Roman numeral string (null‑terminated) */
} entry_t;

/* We use a single allocation for the entries and a static array of
   sorted numbers. The sorted numbers are stored as uint16_t, which
   is small enough for 1..3999 and can be safely cast to seq_t at
   return time. */
static entry_t      *entries  = NULL;   /* 3999 entries */
static uint16_t     sorted_num[3999];   /* sorted integer values */

/* ------------------------------------------------------------- */
/* Initialize the static data on first call.                     */
static void init_sequence(void)
{
    static int initialized = 0;
    if (initialized)
        return;
    initialized = 1;

    entries = (entry_t *)malloc(3999 * sizeof(entry_t));
    if (!entries)    /* allocation failed – leave all values zero */
        return;

    for (int i = 1; i <= 3999; ++i) {
        entries[i-1].num = i;
        int_to_roman(i, entries[i-1].roman);
    }

    /* Sort the pointers to the roman strings in reverse lexicographic
       order.  We sort an array of pointers to ease memory use and
       leave the original entries in place. */
    const char *roman_ptrs[3999];
    for (int i = 0; i < 3999; ++i)
        roman_ptrs[i] = entries[i].roman;

    qsort(roman_ptrs, 3999, sizeof(roman_ptrs[0]), cmp_reverse);

    /* Build the sorted number list from the sorted roman pointers. */
    for (int i = 0; i < 3999; ++i) {
        /* Find the original entry matching this roman string. */
        for (int j = 0; j < 3999; ++j) {
            if (entries[j].roman == roman_ptrs[i] ||
                strcmp(entries[j].roman, roman_ptrs[i]) == 0) {
                sorted_num[i] = (uint16_t)entries[j].num;
                break;
            }
        }
    }
}

/* ------------------------------------------------------------- */
/* Public function required by the harness.                       */
seq_t a(idx_t n)
{
    /* a(1) is the first element of the sorted list.  The sequence
       is defined only for 1 ≤ n ≤ 3999.  For out‑of‑range values
       we return 0. */
    if (n < 1 || n > 3999)
        return (seq_t)0;

    init_sequence();
    return (seq_t)sorted_num[(size_t)n - 1];
}
