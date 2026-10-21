/*
 * A060814  Numbers whose cubes contain more than half the same digit and do
 *          not end in 0.
 * https://oeis.org/A060814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate positive integers not ending in 0, accept those whose cube contains a digit appearing in more than half of the cube’s decimal digits.  Results are cached to serve any order of requests. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t len = 0, cap = 0;
    static idx_t next_num = 1;

    while (len <= n) {
        /* Skip numbers ending in 0 */
        if (next_num % 10 == 0) {
            ++next_num;
            continue;
        }

        seq_t m = (seq_t)next_num;
        /* cube = m * m * m;  multiplication is exact for the available seq_t type */
        seq_t cube = m * m * m;

        unsigned char cnt[10] = {0};
        int total = 0;
        seq_t tmp = cube;
        while (tmp != 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            cnt[d]++;  total++;  tmp /= 10;
        }

        /* Find maximum digit frequency */
        int maxcnt = 0;
        for (int d = 0; d < 10; ++d) {
            if (cnt[d] > maxcnt) maxcnt = cnt[d];
        }

        if (maxcnt * 2 > total) {   /* more than half the digits are the same */
            if (len == cap) {
                cap = cap ? cap * 2 : 16;
                seq_t *newp = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                if (!newp) abort();
                terms = newp;
            }
            terms[len++] = m;
        }
        ++next_num;
    }

    return terms[n];
}
