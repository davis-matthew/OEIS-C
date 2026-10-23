/*
 * A036742  Numbers <= 3999 sorted in Roman numeral lexicographic order.
 * https://oeis.org/A036742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3999, u32 n <= 3999, u64 n <= 3999,
 *            u128 n <= 3999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers <= 3999 sorted by Roman numeral lexicographic order. */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void int_to_roman(unsigned n, char *dest)
{
    static const struct {int val; const char *sym;} table[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100,  "C"}, {90,  "XC"}, {50,  "L"}, {40,  "XL"},
        {10,   "X"}, {9,   "IX"}, {5,   "V"}, {4,   "IV"},
        {1,    "I"}
    };
    const int NT = sizeof(table)/sizeof(table[0]);
    char *p = dest;
    for (int i = 0; i < NT; ++i) {
        while (n >= (unsigned)table[i].val) {
            const char *s = table[i].sym;
            while (*s) *p++ = *s++;
            n -= (unsigned)table[i].val;
        }
    }
    *p = '\0';
}

struct Entry {
    int     val;
    char    s[12];
};

static int cmp_entry(const void *a, const void *b)
{
    const struct Entry *ea = (const struct Entry *)a;
    const struct Entry *eb = (const struct Entry *)b;
    return strcmp(ea->s, eb->s);
}

seq_t a(idx_t n)
{
    enum { N = 3999 };
    static bool initialized = false;
    static int sorted[N];

    if (!initialized) {
        static struct Entry entries[N];
        for (unsigned i = 1; i <= (unsigned)N; ++i) {
            entries[i-1].val = (int)i;
            int_to_roman((unsigned)i, entries[i-1].s);
        }
        qsort(entries, N, sizeof(entries[0]), cmp_entry);
        for (unsigned i = 0; i < (unsigned)N; ++i) {
            sorted[i] = entries[i].val;
        }
        initialized = true;
    }

    if (n >= 1 && n <= (idx_t)N)
        return (seq_t)sorted[(unsigned)(n - 1)];
    return (seq_t)0;   /* out of range */
}
