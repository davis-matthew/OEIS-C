/*
 * A134279  A certain partition array in Abramowitz-Stegun order (A-St order),
 *          called M_3(6)/M_3.
 * https://oeis.org/A134279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = product of A008548(part) over the k‑th partition of * n
 *            in Abramowitz–Stegun (A‑St) order, flattened by rows of length *
 *            p(n) (the partition numbers). The function returns the n‑th term
 *            * of OEIS sequence A134279
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   find_n_pos, gen_by_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/find_n_pos.h"
#include "../utils/gen_by_len.h"

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* out of range */

    uint64_t m, k;
    find_n_pos((uint64_t)n, &m, &k);

    /* pre‑compute 5‑factorials A008548(i) for 0<=i<=m */
    seq_t *prodFact = (seq_t *)malloc((m + 1) * sizeof(seq_t));
    if (!prodFact) exit(1);
    prodFact[0] = (seq_t)1;
    for (uint64_t i = 1; i <= m; ++i) {
        seq_t factor = (seq_t)(5 * (i - 1) + 1);
        prodFact[i] = prodFact[i - 1] * factor;
    }

    seq_t result = (seq_t)0;
    uint64_t count = 0;
    bool found = false;

    /* enumerate by number of parts ascending */
    for (int len = 1; len <= m; ++len) {
        gen_by_len((int)m, len, (int)m, (seq_t)1,
                   &count, k, &result, &found, prodFact);
        if (found) break;
    }

    free(prodFact);
    return result;
}
