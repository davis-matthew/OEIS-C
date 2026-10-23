/*
 * A145752  a(n) = number of elements of order n in simple group Alt(7) of
 *          order 2520.
 * https://oeis.org/A145752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of elements of order n in Alt(7) computed by
 * enumerating all even permutation cycle types of S7.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/gcd.h"

static void gen_parts_rec(int remaining, int max_part, int *parts, int depth,
                          seq_t target, seq_t *result, seq_t fact7)
{
    if (remaining == 0) {
        int cycles = depth;
        if ((7 - cycles) & 1) {          /* odd permutation → skip */
            return;
        }

        int count_len[8] = {0};          /* indices 1..7 */
        for (int i = 0; i < depth; ++i) {
            ++count_len[parts[i]];
        }

        seq_t denom = 1;
        for (int l = 1; l <= 7; ++l) {
            int m = count_len[l];
            if (m > 0) {
                denom *= factorial((idx_t)m);   /* m!   */
                for (int k = 0; k < m; ++k) {   /* l^m  */
                    denom *= (seq_t)l;
                }
            }
        }

        seq_t perm_count = fact7 / denom;

        seq_t ord = 1;
        for (int i = 0; i < depth; ++i) {
            seq_t g = gcd(ord, (seq_t)parts[i]);
            ord = (ord / g) * (seq_t)parts[i];
        }

        if (ord == target) {
            *result += perm_count;
        }
        return;
    }

    for (int k = (remaining < max_part ? remaining : max_part); k >= 1; --k) {
        parts[depth] = k;
        gen_parts_rec(remaining - k, k, parts, depth + 1, target, result, fact7);
    }
}

seq_t a(idx_t n)
{
    /* For the alternating group A7 the maximal order of an element is 7. */
    if (n > 7) {
        return 0;
    }

    static bool fact_init = false;
    static seq_t fact7 = 0;
    if (!fact_init) {
        fact7 = factorial(7);
        fact_init = true;
    }

    seq_t res = 0;
    int parts[7];
    gen_parts_rec(7, 7, parts, 0, (seq_t)n, &res, fact7);
    return res;
}
