/*
 * A101168  Lengths of successive words (starting with a) under the
 *          substitution: {a -> aab, b -> aac, c -> a}.
 * https://oeis.org/A101168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=9, a(n)=2*a(n-1)+2*a(n-2)+a(n-3)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *table = NULL;
static idx_t sz = 0;
static idx_t cap = 0;

static void ensure_capacity(idx_t need)
{
    if (need <= cap) return;
    idx_t newcap = (cap > 0 ? cap * 2 : 4);
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(table, newcap * sizeof(seq_t));
    if (!tmp) abort();                 /* memory exhausted */
    table = tmp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < sz && table != NULL) return table[n];

    /* initialise base terms if not already present */
    if (sz == 0) {
        ensure_capacity(3);
        table[0] = (seq_t)1;
        table[1] = (seq_t)3;
        table[2] = (seq_t)9;
        sz = 3;
    }
    if (n < sz) return table[n];

    ensure_capacity(n + 1);
    for (idx_t i = sz; i <= n; ++i) {
        /* i >= 3 because sz >= 3 */
        seq_t val = (seq_t)2 * table[i - 1] + (seq_t)2 * table[i - 2] + table[i - 3];
        table[i] = val;
    }
    sz = n + 1;
    return table[n];
}
