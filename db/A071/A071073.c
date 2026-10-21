/*
 * A071073  Minimal "multiples of 3" set in base 10.
 * https://oeis.org/A071073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) = the nth element of the minimal set of multiples of 3 in
 *            base 10 such that every multiple of 3 contains at least one of
 *            these elements as a contiguous decimal substring. The set is
 *            constructed by scanning multiples of 3 in ascending order and
 *            inserting a number when it is not already covere...
 * Range:     u8 n <= 36, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   contains_substr, uint64_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "../utils/contains_substr.h"
#include "../utils/uint64_to_str.h"

seq_t a(idx_t n)
{
    /* the set contains at most 76 elements */
    enum { MAX_TERMS = 76 };
    static seq_t terms[MAX_TERMS];
    static char term_str[MAX_TERMS][32];
    static int term_cnt = 0;
    static int initialized = 0;

    if (!initialized) {
        /* Construct the minimal set by greedy scan of multiples of 3. */
        uint64_t x = 0;          /* use 64‑bit temporary to generate values */
        while (term_cnt < MAX_TERMS) {
            char s[32];
            uint64_to_str(x, s);

            int covered = 0;
            for (int i = 0; i < term_cnt; i++) {
                if (contains_substr(s, term_str[i])) { covered = 1; break; }
            }

            if (!covered) {
                terms[term_cnt] = (seq_t)x;        /* cast is safe; terms fit in 64‑bits */
                strcpy(term_str[term_cnt], s);
                term_cnt++;
            }

            x += 3;   /* next multiple of 3 */
        }
        initialized = 1;
    }

    if (n == 0) return 0;  /* undefined for n<1, but return 0 */
    if ((idx_t)(n - 1) < term_cnt)
        return terms[n - 1];
    /* for n greater than the known 76 terms, the definition is not applicable */
    return 0;
}
