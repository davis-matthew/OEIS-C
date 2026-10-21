/*
 * A074233  Numbers whose base-4 and base-5 representations are permutations
 *          of the same multiset of digits.
 * https://oeis.org/A074233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = nth smallest non‑negative integer whose base‑4 *
 *            representation and base‑5 representation are permutations of the
 *            * same multiset of decimal digits. The sequence is finite; all
 *            terms * satisfy 0 ≤ a(n) ≤ 4⁸ − 1 = 65535
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms = NULL;        /* sorted list of valid numbers */
static size_t  terms_len = 0;      /* number of terms found */
static size_t  terms_cap = 0;      /* capacity of terms array */

static void add_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 8;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);       /* allocation failure: abort */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

static void init_terms(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    const idx_t MAX_VAL = ((idx_t)1 << 16) - 1; /* 65535 */
    for (idx_t x = 0; x <= MAX_VAL; ++x) {
        /* Count digits in base 4 */
        unsigned cnt4[10] = {0};
        seq_t tmp = (seq_t)x;
        if (tmp == 0) cnt4[0] = 1;
        while (tmp > 0) {
            int d = (int)(tmp % 4);
            ++cnt4[d];
            tmp /= 4;
        }

        /* Count digits in base 5 */
        unsigned cnt5[10] = {0};
        tmp = (seq_t)x;
        if (tmp == 0) cnt5[0] = 1;
        while (tmp > 0) {
            int d = (int)(tmp % 5);
            ++cnt5[d];
            tmp /= 5;
        }

        /* Base‑4 digits cannot contain 4; if base‑5 contains 4,
         * the multisets cannot match.  Also compare the full
         * multisets (digits 0–9). */
        if (cnt5[4] != 0) continue;    /* impossible match */
        int same = 1;
        for (int d = 0; d <= 9; ++d) {
            if (cnt4[d] != cnt5[d]) { same = 0; break; }
        }
        if (same) add_term((seq_t)x);
    }
}

seq_t a(idx_t n)
{
    init_terms();           /* ensure the table is built once */
    if (n == 0) return 0;   /* offset is 1; a(1) is the first term */
    if (n <= terms_len) return terms[n-1];
    return 0;               /* beyond the finite list */
}
